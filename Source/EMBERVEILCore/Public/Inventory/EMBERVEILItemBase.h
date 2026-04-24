#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Inventory/EMBERVEILItemTypes.h"
#include "GameFramework/EMBERVEILGameInstance.h"
#include "EMBERVEILItemBase.generated.h"

class UGameplayEffect;
class UTexture2D;
class UStaticMesh;

/**
 * DataAsset base para todos los items de EMBERVEIL.
 * Define las propiedades estáticas (inmutables) del item.
 * El estado mutable en runtime vive en UEMBERVEILItemInstance.
 *
 * Crear subclases en Blueprint para cada item concreto del juego.
 */
UCLASS(BlueprintType, Blueprintable)
class EMBERVEILCORE_API UEMBERVEILItemBase : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UEMBERVEILItemBase();

    // ─── IDENTIDAD ────────────────────────────────────────────────────────────

    /** Identificador único del item (p.ej. "SWORD_IRON_01"). Usado para búsquedas en inventario. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Identity")
    FName ItemID;

    /** Nombre localizable mostrado en la UI */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Identity")
    FText ItemName;

    /** Descripción localizable mostrada en el tooltip */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Identity")
    FText ItemDescription;

    // ─── CLASIFICACIÓN ────────────────────────────────────────────────────────

    /** Categoría funcional del item */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Classification")
    EEMBERVEILItemType ItemType = EEMBERVEILItemType::Material;

    /** Rareza del item (afecta color del nombre y calidad de stats) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Classification")
    EEMBERVEILItemRarity Rarity = EEMBERVEILItemRarity::Common;

    // ─── VISUALES ─────────────────────────────────────────────────────────────

    /** Icono 2D del item para la UI del inventario */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Visuals")
    TSoftObjectPtr<UTexture2D> Icon;

    /** Mesh 3D del item cuando está en el mundo (drop en suelo, etc.) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Visuals")
    TSoftObjectPtr<UStaticMesh> WorldMesh;

    // ─── STACKING Y EQUIPAMIENTO ──────────────────────────────────────────────

    /** Cantidad máxima por slot. 1 para equipables, 99 para consumibles/materiales. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Stacking", meta = (ClampMin = "1", ClampMax = "999"))
    int32 MaxStackSize = 1;

    /** Si true, este item puede ser equipado en un slot de equipo */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment")
    bool bIsEquipable = false;

    /** Slot de equipo donde va este item (ignorado si bIsEquipable == false) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment",
        meta = (EditCondition = "bIsEquipable"))
    EEMBERVEILEquipmentSlot EquipmentSlot = EEMBERVEILEquipmentSlot::None;

    // ─── STATS ────────────────────────────────────────────────────────────────

    /** Lista de modificadores de stats que este item otorga al equiparse */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Stats")
    TArray<FItemStatModifier> StatModifiers;

    // ─── GAMEPLAY EFFECTS ─────────────────────────────────────────────────────

    /**
     * GameplayEffect aplicado al ASC del portador cuando se equipa el item.
     * Debe ser de duración Infinite para que persista mientras está equipado.
     * Se elimina al desequipar.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|GameplayEffects")
    TSubclassOf<UGameplayEffect> OnEquipEffect;

    /**
     * GameplayEffect aplicado al ASC del portador cuando se usa el item.
     * Típicamente Instant o HasDuration para consumibles.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|GameplayEffects")
    TSubclassOf<UGameplayEffect> OnUseEffect;

    // ─── REQUISITOS Y ECONOMÍA ────────────────────────────────────────────────

    /** Nivel mínimo del personaje para poder equipar/usar este item */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Requirements", meta = (ClampMin = "1"))
    int32 RequiredLevel = 1;

    /** Valor base del item en monedas del juego (precio de venta base) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Economy", meta = (ClampMin = "0"))
    int32 BaseValue = 0;

    // ─── MÉTODOS ──────────────────────────────────────────────────────────────

    /**
     * Determina si este item puede ser usado por un personaje con la clase y raza dadas.
     * Por defecto devuelve true — sobreescribir en Blueprint para restricciones específicas.
     *
     * @param CharClass  Clase del personaje que intenta usar el item
     * @param Race       Raza del personaje que intenta usar el item
     * @return           true si el personaje puede usar este item
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item|Requirements")
    bool CanBeUsedBy(EEMBERVEILClass CharClass, EEMBERVEILRace Race) const;
    virtual bool CanBeUsedBy_Implementation(EEMBERVEILClass CharClass, EEMBERVEILRace Race) const;

    /** Devuelve el FPrimaryAssetId usado por el AssetManager para cargar este DataAsset */
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
