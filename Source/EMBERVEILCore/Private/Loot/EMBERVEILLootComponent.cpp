#include "Loot/EMBERVEILLootComponent.h"

#include "Loot/EMBERVEILLootTableDataAsset.h"
#include "Loot/AEMBERVEILWorldItem.h"
#include "Loot/EMBERVEILLootTypes.h"
#include "Character/EMBERVEILCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

UEMBERVEILLootComponent::UEMBERVEILLootComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

// ─── BeginPlay ────────────────────────────────────────────────────────────────

void UEMBERVEILLootComponent::BeginPlay()
{
    Super::BeginPlay();

    // Solo el servidor gestiona loot — en clientes no hace nada.
    if (!GetOwner() || !GetOwner()->HasAuthority())
        return;

    if (!bAutoSpawnOnDeath)
        return;

    // Obtenemos el ASC del owner y escuchamos el tag State.Dead.
    // Este patrón refleja exactamente cómo CharacterBase detecta su propia muerte
    // (ver BindGASCallbacks en EMBERVEILCharacterBase.cpp).
    IAbilitySystemInterface* ASInterface = Cast<IAbilitySystemInterface>(GetOwner());
    if (!ASInterface)
        return;

    UAbilitySystemComponent* OwnerASC = ASInterface->GetAbilitySystemComponent();
    if (!OwnerASC)
        return;

    const FGameplayTag DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));

    // Guardamos el handle para poder desregistrar si el componente se destruye.
    DeathTagHandle = OwnerASC->RegisterGameplayTagEvent(
        DeadTag,
        EGameplayTagEventType::NewOrRemoved
    ).AddLambda([this](const FGameplayTag /*Tag*/, int32 NewCount)
    {
        // NewCount > 0 significa que el tag fue añadido (el personaje acaba de morir).
        if (NewCount > 0)
        {
            OnOwnerDied(nullptr);
        }
    });
}

// ─── OnOwnerDied ──────────────────────────────────────────────────────────────

void UEMBERVEILLootComponent::OnOwnerDied(AActor* Killer)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
        return;

    if (!bAutoSpawnOnDeath)
        return;

    SpawnLoot(GetOwner()->GetActorLocation());
}

// ─── SpawnLoot ────────────────────────────────────────────────────────────────

void UEMBERVEILLootComponent::SpawnLoot(FVector SpawnLocation)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
        return;

    if (!LootTable)
        return;

    UWorld* World = GetWorld();
    if (!World)
        return;

    // Tiramos la tabla de loot (LuckBonus = 0 por defecto; se puede exponer
    // como parámetro en una versión futura que lea el atributo Luck del killer).
    TArray<FLootResult> LootResults = LootTable->RollLoot(0.f);

    // Parámetros de spawn comunes: sin colisión al spawner para evitar penetraciones.
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // ── Spawnear items ────────────────────────────────────────────────────────
    for (const FLootResult& Result : LootResults)
    {
        if (!Result.Item || Result.Quantity <= 0)
            continue;

        const FVector ItemLocation  = SpawnLocation + GetRandomOffsetInRadius();
        const FRotator ItemRotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);

        AEMBERVEILWorldItem* WorldItem = World->SpawnActor<AEMBERVEILWorldItem>(
            AEMBERVEILWorldItem::StaticClass(),
            ItemLocation,
            ItemRotation,
            SpawnParams
        );

        if (WorldItem)
        {
            WorldItem->InitializeItem(Result.Item, Result.Quantity, ItemLifetime);
        }
    }

    // ── Spawnear moneda ───────────────────────────────────────────────────────
    const int32 CurrencyAmount = LootTable->RollCurrency();
    if (CurrencyAmount > 0)
    {
        const FVector CurrencyLocation  = SpawnLocation + GetRandomOffsetInRadius();
        const FRotator CurrencyRotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);

        AEMBERVEILWorldItem* CurrencyItem = World->SpawnActor<AEMBERVEILWorldItem>(
            AEMBERVEILWorldItem::StaticClass(),
            CurrencyLocation,
            CurrencyRotation,
            SpawnParams
        );

        if (CurrencyItem)
        {
            CurrencyItem->InitializeCurrency(CurrencyAmount, ItemLifetime);
        }
    }
}

// ─── Helpers ──────────────────────────────────────────────────────────────────

FVector UEMBERVEILLootComponent::GetRandomOffsetInRadius() const
{
    // Distribución uniforme en círculo usando coordenadas polares.
    const float Angle    = FMath::FRandRange(0.f, 2.f * PI);
    // Raíz cuadrada del radio aleatorio para distribución uniforme en área
    // (sin raíz cuadrada los items se acumulan en el centro).
    const float Distance = LootRadius * FMath::Sqrt(FMath::FRand());

    return FVector(Distance * FMath::Cos(Angle), Distance * FMath::Sin(Angle), 0.f);
}
