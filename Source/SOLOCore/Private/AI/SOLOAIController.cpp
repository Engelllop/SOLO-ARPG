#include "AI/SOLOAIController.h"
#include "Character/SOLOEnemyCharacter.h"
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

// Static key definitions â€” single source of truth for all BT nodes
const FName ASOLOAIController::BB_TargetActor  = TEXT("TargetActor");
const FName ASOLOAIController::BB_bIsAggro     = TEXT("bIsAggro");
const FName ASOLOAIController::BB_PatrolOrigin = TEXT("PatrolOrigin");
const FName ASOLOAIController::BB_AttackRange  = TEXT("AttackRange");
const FName ASOLOAIController::BB_DistanceToTarget = TEXT("DistanceToTarget");
const FName ASOLOAIController::BB_bCanAttack   = TEXT("bCanAttack");
const FName ASOLOAIController::BB_CurrentPhase = TEXT("CurrentPhase");

ASOLOAIController::ASOLOAIController()
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

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ASOLOAIController::OnTargetPerceptionUpdated);

	StunnedTag = FGameplayTag::RequestGameplayTag(FName("State.Stunned"), false);
}

void ASOLOAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ASOLOEnemyCharacter* EnemyChar = Cast<ASOLOEnemyCharacter>(InPawn);
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

void ASOLOAIController::OnUnPossess()
{
	UnbindStunnedTagDelegate();

	GetWorldTimerManager().ClearTimer(SearchTimer);
	BehaviorTreeComponent->StopTree();
	Super::OnUnPossess();
}

void ASOLOAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor)
	{
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		// Cancel any ongoing "lost sight" search timer â€” we found them again
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
				&ASOLOAIController::OnSearchTimerExpired,
				SearchDuration,
				false
			);
		}
	}
}

void ASOLOAIController::SetTarget(AActor* NewTarget)
{
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsObject(BB_TargetActor, NewTarget);
	}

	// Mirror on the pawn so non-BT systems can query it cheaply
	if (ASOLOEnemyCharacter* EnemyChar = Cast<ASOLOEnemyCharacter>(GetPawn()))
	{
		EnemyChar->SetCombatFocus(NewTarget);
	}
}

void ASOLOAIController::ClearTarget()
{
	GetWorldTimerManager().ClearTimer(SearchTimer);

	if (BlackboardComponent)
	{
		BlackboardComponent->ClearValue(BB_TargetActor);
		BlackboardComponent->SetValueAsFloat(BB_DistanceToTarget, -1.f);
		BlackboardComponent->SetValueAsBool(BB_bIsAggro, false);
	}

	if (ASOLOEnemyCharacter* EnemyChar = Cast<ASOLOEnemyCharacter>(GetPawn()))
	{
		EnemyChar->SetCombatFocus(nullptr);
	}
}

void ASOLOAIController::OnSearchTimerExpired()
{
	// Search phase ended without re-acquiring target â€” go back to patrol
	ClearTarget();
}

void ASOLOAIController::BindStunnedTagDelegate(APawn* InControlledPawn)
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
		.AddUObject(this, &ASOLOAIController::OnStunnedTagChanged);
}

void ASOLOAIController::UnbindStunnedTagDelegate()
{
	if (StunnedTagASC.IsValid() && StunnedTag.IsValid())
	{
		StunnedTagASC->RegisterGameplayTagEvent(StunnedTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	}
	StunnedTagASC = nullptr;
}

void ASOLOAIController::OnStunnedTagChanged(FGameplayTag Tag, int32 NewCount)
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
