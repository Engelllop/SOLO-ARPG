#include "Loot/SOLOLootComponent.h"

#include "Loot/SOLOLootTableDataAsset.h"
#include "Loot/ASOLOWorldItem.h"
#include "Loot/SOLOLootTypes.h"
#include "Character/SOLOCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

USOLOLootComponent::USOLOLootComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

// â”€â”€â”€ BeginPlay â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void USOLOLootComponent::BeginPlay()
{
    Super::BeginPlay();

    // Solo el servidor gestiona loot â€” en clientes no hace nada.
    if (!GetOwner() || !GetOwner()->HasAuthority())
        return;

    if (!bAutoSpawnOnDeath)
        return;

    // Obtenemos el ASC del owner y escuchamos el tag State.Dead.
    // Este patrÃ³n refleja exactamente cÃ³mo CharacterBase detecta su propia muerte
    // (ver BindGASCallbacks en SOLOCharacterBase.cpp).
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
        // NewCount > 0 significa que el tag fue aÃ±adido (el personaje acaba de morir).
        if (NewCount > 0)
        {
            OnOwnerDied(nullptr);
        }
    });
}

// â”€â”€â”€ OnOwnerDied â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void USOLOLootComponent::OnOwnerDied(AActor* Killer)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
        return;

    if (!bAutoSpawnOnDeath)
        return;

    SpawnLoot(GetOwner()->GetActorLocation());
}

// â”€â”€â”€ SpawnLoot â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void USOLOLootComponent::SpawnLoot(FVector SpawnLocation)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
        return;

    if (!LootTable)
        return;

    UWorld* World = GetWorld();
    if (!World)
        return;

    // Tiramos la tabla de loot (LuckBonus = 0 por defecto; se puede exponer
    // como parÃ¡metro en una versiÃ³n futura que lea el atributo Luck del killer).
    TArray<FLootResult> LootResults = LootTable->RollLoot(0.f);

    // ParÃ¡metros de spawn comunes: sin colisiÃ³n al spawner para evitar penetraciones.
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // â”€â”€ Spawnear items â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    for (const FLootResult& Result : LootResults)
    {
        if (!Result.Item || Result.Quantity <= 0)
            continue;

        const FVector ItemLocation  = SpawnLocation + GetRandomOffsetInRadius();
        const FRotator ItemRotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);

        ASOLOWorldItem* WorldItem = World->SpawnActor<ASOLOWorldItem>(
            ASOLOWorldItem::StaticClass(),
            ItemLocation,
            ItemRotation,
            SpawnParams
        );

        if (WorldItem)
        {
            WorldItem->InitializeItem(Result.Item, Result.Quantity, ItemLifetime);
        }
    }

    // â”€â”€ Spawnear moneda â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    const int32 CurrencyAmount = LootTable->RollCurrency();
    if (CurrencyAmount > 0)
    {
        const FVector CurrencyLocation  = SpawnLocation + GetRandomOffsetInRadius();
        const FRotator CurrencyRotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);

        ASOLOWorldItem* CurrencyItem = World->SpawnActor<ASOLOWorldItem>(
            ASOLOWorldItem::StaticClass(),
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

// â”€â”€â”€ Helpers â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

FVector USOLOLootComponent::GetRandomOffsetInRadius() const
{
    // DistribuciÃ³n uniforme en cÃ­rculo usando coordenadas polares.
    const float Angle    = FMath::FRandRange(0.f, 2.f * PI);
    // RaÃ­z cuadrada del radio aleatorio para distribuciÃ³n uniforme en Ã¡rea
    // (sin raÃ­z cuadrada los items se acumulan en el centro).
    const float Distance = LootRadius * FMath::Sqrt(FMath::FRand());

    return FVector(Distance * FMath::Cos(Angle), Distance * FMath::Sin(Angle), 0.f);
}
