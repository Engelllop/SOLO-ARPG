#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "Data/SOLOEnums.h"
#include "SOLOCompanionAIController.generated.h"

UCLASS()
class SOLOCORE_API ASOLOCompanionAIController : public AAIController
{
	GENERATED_BODY()
public:
	ASOLOCompanionAIController();

	void SetCommand(ESOLOCompanionCommand NewCommand);
	ESOLOCompanionCommand GetCurrentCommand() const { return CurrentCommand; }

	void SetFollowTarget(AActor* Target);
	void SetAggroTarget(AActor* Target);

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly) UBehaviorTree* CompanionBehaviorTree;

private:
	ESOLOCompanionCommand CurrentCommand = ESOLOCompanionCommand::Attack;

	UPROPERTY(Transient) AActor* FollowTarget = nullptr;
	UPROPERTY(Transient) AActor* AggroTarget  = nullptr;

	static const FName BB_FollowTarget;
	static const FName BB_AggroTarget;
	static const FName BB_Command;
};
