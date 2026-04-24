#include "AI/BehaviorTree/Services/BTService_RotateToTarget.h"
#include "AI/EMBERVEILAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"

UBTService_RotateToTarget::UBTService_RotateToTarget()
{
	NodeName = TEXT("Rotate To Target");
	// 20 ticks/sec gives visually smooth rotation without hammering the BT
	Interval        = 0.05f;
	RandomDeviation = 0.f;
}

void UBTService_RotateToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
	{
		return;
	}

	// Only rotate while aggressive — during patrol it looks weird to face a ghost target
	if (!BB->GetValueAsBool(AEMBERVEILAIController::BB_bIsAggro))
	{
		return;
	}

	const AActor* Target = Cast<AActor>(BB->GetValueAsObject(AEMBERVEILAIController::BB_TargetActor));
	if (!Target)
	{
		return;
	}

	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn*         Pawn = AIC ? AIC->GetPawn() : nullptr;
	if (!Pawn)
	{
		return;
	}

	const FVector  ToTarget    = (Target->GetActorLocation() - Pawn->GetActorLocation()).GetSafeNormal();
	const FRotator TargetRot   = ToTarget.Rotation();
	const FRotator CurrentRot  = Pawn->GetActorRotation();

	// Interpolate Yaw only — we never want the enemy to tilt pitch toward the player
	FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaSeconds, RotationSpeed);
	NewRot.Pitch = CurrentRot.Pitch;
	NewRot.Roll  = CurrentRot.Roll;

	Pawn->SetActorRotation(NewRot);
}

FString UBTService_RotateToTarget::GetStaticDescription() const
{
	return FString::Printf(TEXT("Rotate To Target | Speed: %.1f"), RotationSpeed);
}
