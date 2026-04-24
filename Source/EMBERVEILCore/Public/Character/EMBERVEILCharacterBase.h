#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/ICombatInterface.h"
#include "GameplayTagContainer.h"
#include "EMBERVEILCharacterBase.generated.h"

class UAbilitySystemComponent;
class UEMBERVEILAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UAnimMontage;
class UNiagaraSystem;
class UWG_DamageNumber;

/**
 * Clase base para todos los personajes de EMBERVEIL (jugador, enemigos, NPCs).
 * Implementa IAbilitySystemInterface para GAS y ICombatInterface para combate.
 *
 * JUGADOR: el ASC vive en PlayerState (sobrevive al respawn).
 * ENEMIGOS: el ASC vive en esta clase directamente.
 */
UCLASS(Abstract)
class EMBERVEILCORE_API AEMBERVEILCharacterBase
	: public ACharacter
	, public IAbilitySystemInterface
	, public ICombatInterface
{
	GENERATED_BODY()

public:
	AEMBERVEILCharacterBase();

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// ICombatInterface
	virtual int32 GetCharacterLevel_Implementation() const override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag) const override;
	virtual bool IsDead_Implementation() const override;
	virtual void Die_Implementation(AActor* InstigatorActor) override;
	virtual USkeletalMeshComponent* GetCombatMesh_Implementation() const override;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	UEMBERVEILAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	virtual void BeginPlay() override;

	// Inicializa los atributos base vía GameplayEffect (en el GE se definen los valores por raza/clase)
	void ApplyDefaultAttributes() const;

	// Concede las habilidades base del personaje al ASC
	void GrantDefaultAbilities();

	// Punto de anclaje en el mesh del arma para efectos de impacto / proyectiles
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName WeaponTipSocketName = TEXT("WeaponTip");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FName LeftHandSocketName = TEXT("LeftHandSocket");

	// GE que inicializa los atributos al comenzar (HP, Mana, stats primarios)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Init")
	TSubclassOf<UGameplayEffect> DefaultAttributesEffect;

	// GEs secundarios (stats derivados calculados desde los primarios)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Init")
	TArray<TSubclassOf<UGameplayEffect>> SecondaryAttributesEffects;

	// Habilidades que se conceden al comenzar (ataque básico, dash, etc.)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	// Tags que describen el estado del personaje
	UPROPERTY(BlueprintReadOnly, Category = "GAS|Tags")
	FGameplayTag DeadTag;

	// Niagara a instanciar en el punto de impacto al recibir daño
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	TObjectPtr<UNiagaraSystem> HitEffect;

	// Niagara de muerte
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	TObjectPtr<UNiagaraSystem> DeathEffect;

	// Montaje de animación de muerte
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UWG_DamageNumber> DamageNumberClass;

	// GAS components — el jugador los obtiene del PlayerState, los enemigos los tienen aquí
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UEMBERVEILAttributeSet> AttributeSet;

	bool bDead = false;

	/** Evita registrar dos veces el listener de State.Dead (BeginPlay + InitAbilityActorInfo en jugador). */
	bool bGASTagDeathCallbackBound = false;

	// Bind de callbacks de GAS (muerte, hit reactions)
	void BindGASCallbacks();
};
