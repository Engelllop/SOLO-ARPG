#include "AI/EMBERVEILAIController.h"
#include "Character/EMBERVEILEnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "BrainComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/ICombatInterface.h"

// Static key definitions — single source of truth for all BT nodes
const FName AEMBERVEILAIController::BB_TargetActor  = TEXT("TargetActor");
const FName AEMBERVEILAIController::BB_bIsAggro     = TEXT("bIsAggro");
const FName AEMBERVEILAIController::BB_PatrolOrigin = TEXT("PatrolOrigin");
const FName AEMBERVEILAIController::BB_AttackRange  = TEXT("AttackRange");
const FName AEMBERVEILAIController::BB_DistanceToTarget = TEXT("DistanceToTarget");
const FName AEMBERVEILAIController::BB_bCanAttack   = TEXT("bCanAttack");
const FName AEMBERVEILAIController::BB_CurrentPhase = TEXT("CurrentPhase");

AEMBERVEILAIController::AEMBERVEILAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent   = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	// Sight: detects the player (tagged as Enemy from the AI's faction perspective)
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius                 = 1200.f;
	SightConfig->LoseSightRadius             = 800.f;   // peripheral radius
	SightConfig->PeripheralVisionAngleDegrees = 90.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->DetectionByAffiliation.bDetectEnemies   = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals  = false;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

	// Hearing: footsteps and attack sounds propagated by UAISense_Hearing noise emitters
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 800.f;
	HearingConfig->SetMaxAge(3.f);
	HearingConfig->DetectionByAffiliation.bDetectEnemies   = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals  = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->ConfigureSense(*HearingConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEMBERVEILAIController::OnTargetPerceptionUpdated);

	StunnedTag = FGameplayTag::RequestGameplayTag(FName("State.Stunned"), false);
}

void AEMBERVEILAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AEMBERVEILEnemyCharacter* EnemyChar = Cast<AEMBERVEILEnemyCharacter>(InPawn);
	if (!EnemyChar || !BehaviorTree)
	{
		return;
	}

	if (UBlackboardData* BBData = BehaviorTree->BlackboardAsset)
	{
		UBlackboardComponent* BBComp = BlackboardComponent.Get();
		UseBlackboard(BBData, BBComp);
		BlackboardComponent = BBComp;
	}

	// Seed patrol origin at spawn so the enemy always knows its home position
	BlackboardComponent->SetValueAsVector(BB_PatrolOrigin, InPawn->GetActorLocation());
	BlackboardComponent->SetValueAsFloat(BB_AttackRange, 150.f);   // default; overridden per enemy type in BP
	BlackboardComponent->SetValueAsFloat(BB_DistanceToTarget, -1.f);
	BlackboardComponent->SetValueAsBool(BB_bIsAggro, false);
	BlackboardComponent->SetValueAsBool(BB_bCanAttack, true);
	BlackboardComponent->SetValueAsInt(BB_CurrentPhase, 0);

	RunBehaviorTree(BehaviorTree);

	BindStunnedTagDelegate(InPawn);
}

void AEMBERVEILAIController::OnUnPossess()
{
	UnbindStunnedTagDelegate();

	GetWorldTimerManager().ClearTimer(SearchTimer);
	BehaviorTreeComponent->StopTree();
	Super::OnUnPossess();
}

void AEMBERVEILAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor)
	{
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		// Cancel any ongoing "lost sight" search timer — we found them again
		GetWorldTimerManager().ClearTimer(SearchTimer);
		SetTarget(Actor);
		BlackboardComponent->SetValueAsBool(BB_bIsAggro, true);
	}
	else
	{
		// Sight lost: enter search phase before returning to patrol
		if (!GetWorldTimerManager().IsTimerActive(SearchTimer))
		{
			GetWorldTimerManager().SetTimer(
				SearchTimer,
				this,
				&AEMBERVEILAIController::OnSearchTimerExpired,
				SearchDuration,
				false
			);
		}
	}
}

void AEMBERVEILAIController::SetTarget(AActor* NewTarget)
{
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsObject(BB_TargetActor, NewTarget);
	}

	// Mirror on the pawn so non-BT systems can query it cheaply
	if (AEMBERVEILEnemyCharacter* EnemyChar = Cast<AEMBERVEILEnemyCharacter>(GetPawn()))
	{
		EnemyChar->SetCombatFocus(NewTarget);
	}
}

void AEMBERVEILAIController::ClearTarget()
{
	GetWorldTimerManager().ClearTimer(SearchTimer);

	if (BlackboardComponent)
	{
		BlackboardComponent->ClearValue(BB_TargetActor);
		BlackboardComponent->SetValueAsFloat(BB_DistanceToTarget, -1.f);
		BlackboardComponent->SetValueAsBool(BB_bIsAggro, false);
	}

	if (AEMBERVEILEnemyCharacter* EnemyChar = Cast<AEMBERVEILEnemyCharacter>(GetPawn()))
	{
		EnemyChar->SetCombatFocus(nullptr);
	}
}

void AEMBERVEILAIController::OnSearchTimerExpired()
{
	// Search phase ended without re-acquiring target — go back to patrol
	ClearTarget();
}

void AEMBERVEILAIController::BindStunnedTagDelegate(APawn* InControlledPawn)
{
	UnbindStunnedTagDelegate();

	if (!InControlledPawn || !StunnedTag.IsValid())
	{
		return;
	}

	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(InControlledPawn);
	if (!ASI)
	{
		return;
	}

	UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}

	StunnedTagASC = ASC;
	ASC->RegisterGameplayTagEvent(StunnedTag, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AEMBERVEILAIController::OnStunnedTagChanged);
}

void AEMBERVEILAIController::UnbindStunnedTagDelegate()
{
	if (StunnedTagASC.IsValid() && StunnedTag.IsValid())
	{
		StunnedTagASC->RegisterGameplayTagEvent(StunnedTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	}
	StunnedTagASC = nullptr;
}

void AEMBERVEILAIController::OnStunnedTagChanged(FGameplayTag Tag, int32 NewCount)
{
	(void)Tag;
	UBrainComponent* Brain = GetBrainComponent();
	if (!Brain)
	{
		return;
	}

	if (NewCount > 0)
	{
		Brain->StopLogic(TEXT("Stunned"));
	}
	else
	{
		Brain->RestartLogic();
	}
}
