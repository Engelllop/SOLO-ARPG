#include "AI/BehaviorTree/Services/BTService_CombatUpdate.h"
#include "AI/EMBERVEILAIController.h"
#include "Character/EMBERVEILEnemyCharacter.h"
#include "Interfaces/ICombatInterface.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameplayTagsManager.h"

UBTService_CombatUpdate::UBTService_CombatUpdate()
{
	NodeName = TEXT("Combat Update");
	// 5 updates per second — cheap enough to run frequently, sparse enough not to hammer the BT
	Interval          = 0.2f;
	RandomDeviation   = 0.05f;

	Tag_Stunned   = UGameplayTagsManager::Get().RequestGameplayTag(FName("State.Stunned"),   false);
	Tag_Dead      = UGameplayTagsManager::Get().RequestGameplayTag(FName("State.Dead"),       false);
	Tag_Attacking = UGameplayTagsManager::Get().RequestGameplayTag(FName("State.Attacking"),  false);
}

void UBTService_CombatUpdate::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		return;
	}

	AEMBERVEILAIController* AIC = Cast<AEMBERVEILAIController>(OwnerComp.GetAIOwner());
	if (!AIC)
	{
		return;
	}

	AEMBERVEILEnemyCharacter* EnemyChar = Cast<AEMBERVEILEnemyCharacter>(AIC->GetPawn());
	if (!EnemyChar)
	{
		return;
	}

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(AEMBERVEILAIController::BB_TargetActor));

	// Distancia al objetivo (cm); -1 sin objetivo. Debe coincidir con la clave en el Blackboard Asset.
	if (Target)
	{
		const float Dist = FVector::Distance(EnemyChar->GetActorLocation(), Target->GetActorLocation());
		BB->SetValueAsFloat(AEMBERVEILAIController::BB_DistanceToTarget, Dist);
	}
	else
	{
		BB->SetValueAsFloat(AEMBERVEILAIController::BB_DistanceToTarget, -1.f);
	}

	// --- 1. Target alive check ---
	if (Target && Target->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Target))
	{
		AIC->ClearTarget();
		return;
	}

	// --- 2. bCanAttack based on current enemy state tags ---
	bool bCanAttack = true;
	if (const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(EnemyChar))
	{
		if (const UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent())
		{
			bCanAttack = !ASC->HasMatchingGameplayTag(Tag_Dead)
			          && !ASC->HasMatchingGameplayTag(Tag_Stunned)
			          && !ASC->HasMatchingGameplayTag(Tag_Attacking);
		}
	}
	BB->SetValueAsBool(AEMBERVEILAIController::BB_bCanAttack, bCanAttack);

	// --- 3. Aggro lost radius check ---
	const bool bIsAggro = BB->GetValueAsBool(AEMBERVEILAIController::BB_bIsAggro);
	if (bIsAggro && Target)
	{
		const float DistSq          = FVector::DistSquared(EnemyChar->GetActorLocation(), Target->GetActorLocation());
		const float AggroLostRadiusSq = FMath::Square(EnemyChar->GetAggroLostRadius());
		if (DistSq > AggroLostRadiusSq)
		{
			AIC->ClearTarget();
			BB->SetValueAsBool(AEMBERVEILAIController::BB_bIsAggro, false);
		}
	}

	// --- 4. Boss phase sync ---
	if (EnemyChar->IsBossEnemy())
	{
		BB->SetValueAsInt(AEMBERVEILAIController::BB_CurrentPhase, EnemyChar->GetBossCurrentPhase());
	}
}

FString UBTService_CombatUpdate::GetStaticDescription() const
{
	return TEXT("Combat Update");
}
