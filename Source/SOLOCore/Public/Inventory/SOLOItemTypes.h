#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SOLOItemTypes.generated.h"

// Forward declarations
class USOLOItemInstance;

// â”€â”€â”€ ENUMS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

/**
 * CategorÃ­a funcional del item. Determina cÃ³mo se usa y dÃ³nde puede ir en el inventario.
 */
UENUM(BlueprintType)
enum class ESOLOItemType : uint8
{
    Weapon      UMETA(DisplayName = "Weapon"),
    Armor       UMETA(DisplayName = "Armor"),
    Consumable  UMETA(DisplayName = "Consumable"),
    Material    UMETA(DisplayName = "Material"),
    Rune        UMETA(DisplayName = "Rune"),
    QuestItem   UMETA(DisplayName = "Quest Item"),
    Currency    UMETA(DisplayName = "Currency")
};

/**
 * Rareza del item. Controla el color del nombre y la calidad general del loot.
 */
UENUM(BlueprintType)
enum class ESOLOItemRarity : uint8
{
    Common      UMETA(DisplayName = "Common"),
    Uncommon    UMETA(DisplayName = "Uncommon"),
    Rare        UMETA(DisplayName = "Rare"),
    Epic        UMETA(DisplayName = "Epic"),
    Legendary   UMETA(DisplayName = "Legendary"),
    Divine      UMETA(DisplayName = "Divine")
};

/**
 * Slots de equipamiento disponibles para el personaje (12 slots + None).
 */
UENUM(BlueprintType)
enum class ESOLOEquipmentSlot : uint8
{
    None        UMETA(DisplayName = "None"),
    Head        UMETA(DisplayName = "Head"),
    Chest       UMETA(DisplayName = "Chest"),
    Hands       UMETA(DisplayName = "Hands"),
    Legs        UMETA(DisplayName = "Legs"),
    Feet        UMETA(DisplayName = "Feet"),
    MainHand    UMETA(DisplayName = "Main Hand"),
    OffHand     UMETA(DisplayName = "Off Hand"),
    Ring1       UMETA(DisplayName = "Ring 1"),
    Ring2       UMETA(DisplayName = "Ring 2"),
    Neck        UMETA(DisplayName = "Neck"),
    Rune1       UMETA(DisplayName = "Rune Slot 1"),
    Rune2       UMETA(DisplayName = "Rune Slot 2")
};

// â”€â”€â”€ STRUCTS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

/**
 * Modificador de un atributo de GAS que un item puede otorgar al ser equipado.
 * Puede ser valor plano o porcentaje del valor base.
 */
USTRUCT(BlueprintType)
struct SOLOCORE_API FItemStatModifier
{
    GENERATED_BODY()

    /** Atributo de gameplay que se modifica (p.ej. USOLOAttributeSet::GetStrengthAttribute()) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat Modifier")
    FGameplayAttribute Attribute;

    /** Valor del modificador (puede ser negativo para penalizaciones) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat Modifier")
    float Value = 0.f;

    /** Si true, Value se interpreta como porcentaje (0.1 = +10%) en lugar de valor plano */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat Modifier")
    bool bIsPercentage = false;
};

/**
 * Representa un slot del inventario con su instancia de item y cantidad.
 * Usado tanto en la lÃ³gica del componente como en la UI.
 */
USTRUCT(BlueprintType)
struct SOLOCORE_API FInventorySlot
{
    GENERATED_BODY()

    /** Instancia runtime del item (nula si el slot estÃ¡ vacÃ­o) */
    UPROPERTY(BlueprintReadOnly, Category = "Inventory Slot")
    TObjectPtr<USOLOItemInstance> ItemInstance;

    /** Cantidad actual en este slot */
    UPROPERTY(BlueprintReadOnly, Category = "Inventory Slot")
    int32 Quantity = 0;

    /** Devuelve true si el slot no tiene ningÃºn item asignado */
    bool IsEmpty() const { return ItemInstance == nullptr || Quantity <= 0; }
};

/** Par slot + instancia para replicaciÃ³n (TMap no es compatible con UHT + Replicated). */
USTRUCT(BlueprintType)
struct SOLOCORE_API FReplicatedEquipmentEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Equipment")
    ESOLOEquipmentSlot Slot = ESOLOEquipmentSlot::None;

    UPROPERTY(BlueprintReadOnly, Category = "Equipment")
    TObjectPtr<USOLOItemInstance> ItemInstance = nullptr;
};
