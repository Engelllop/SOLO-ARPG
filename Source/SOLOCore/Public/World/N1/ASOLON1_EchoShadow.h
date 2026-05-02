#pragma once
#include "CoreMinimal.h"
#include "Character/SOLOEnemyCharacter.h"
#include "ASOLON1_EchoShadow.generated.h"

UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API ASOLON1_EchoShadow : public ASOLOEnemyCharacter
{
	GENERATED_BODY()
public:
	ASOLON1_EchoShadow();

	UFUNCTION(BlueprintCallable, Category = "N1|Echo")
	void InitEcho(AActor* SourceActor, float AttackDelay);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Echo")
	float DefaultAttackDelay = 2.f;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Echo")
	float DamageAmount = 15.f;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Echo")
	float AttackRadius = 150.f;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Echo")
	float LifespanAfterAttack = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "N1|Echo|GAS")
	TSubclassOf<UGameplayEffect> EchoDamageGE;

protected:
	UFUNCTION(BlueprintImplementableEvent) void BP_OnEchoSpawned();
	UFUNCTION(BlueprintImplementableEvent) void BP_OnEchoAttack();

private:
	FTimerHandle AttackTimer;
	TWeakObjectPtr<AActor> EchoSource;

	UFUNCTION() void ExecuteAttack();
};
