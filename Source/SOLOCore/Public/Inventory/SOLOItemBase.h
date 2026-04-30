#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Inventory/SOLOItemTypes.h"
#include "GameFramework/SOLOGameInstance.h"
#include "SOLOItemBase.generated.h"

class UGameplayEffect;
class UTexture2D;
class UStaticMesh;

/**
 * DataAsset base para todos los items de SOLO.
 * Define las propiedades estÃ¡ticas (inmutables) del item.
 * El estado mutable en runtime vive en USOLOItemInstance.
 *
 * Crear subclases en Blueprint para cada item concreto del juego.
 */
UCLASS(BlueprintType, Blueprintable)
class SOLOCORE_API USOLOItemBase : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    USOLOItemBase();

    // â”€â”€â”€ IDENTIDAD â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Identificador Ãºnico del item (p.ej. "SWORD_IRON_01"). Usado para bÃºsquedas en inventario. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Identity")
    FName ItemID;

    /** Nombre localizable mostrado en la UI */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Identity")
    FText ItemName;

    /** DescripciÃ³n localizable mostrada en el tooltip */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Identity")
    FText ItemDescription;

    // â”€â”€â”€ CLASIFICACIÃ“N â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** CategorÃ­a funcional del item */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Classification")
    ESOLOItemType ItemType = ESOLOItemType::Material;

    /** Rareza del item (afecta color del nombre y calidad de stats) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Classification")
    ESOLOItemRarity Rarity = ESOLOItemRarity::Common;

    // â”€â”€â”€ VISUALES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Icono 2D del item para la UI del inventario */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Visuals")
    TSoftObjectPtr<UTexture2D> Icon;

    /** Mesh 3D del item cuando estÃ¡ en el mundo (drop en suelo, etc.) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Visuals")
    TSoftObjectPtr<UStaticMesh> WorldMesh;

    // â”€â”€â”€ STACKING Y EQUIPAMIENTO â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Cantidad mÃ¡xima por slot. 1 para equipables, 99 para consumibles/materiales. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Stacking", meta = (ClampMin = "1", ClampMax = "999"))
    int32 MaxStackSize = 1;

    /** Si true, este item puede ser equipado en un slot de equipo */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment")
    bool bIsEquipable = false;

    /** Slot de equipo donde va este item (ignorado si bIsEquipable == false) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Equipment",
        meta = (EditCondition = "bIsEquipable"))
    ESOLOEquipmentSlot EquipmentSlot = ESOLOEquipmentSlot::None;

    // â”€â”€â”€ STATS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Lista de modificadores de stats que este item otorga al equiparse */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Stats")
    TArray<FItemStatModifier> StatModifiers;

    // â”€â”€â”€ GAMEPLAY EFFECTS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * GameplayEffect aplicado al ASC del portador cuando se equipa el item.
     * Debe ser de duraciÃ³n Infinite para que persista mientras estÃ¡ equipado.
     * Se elimina al desequipar.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|GameplayEffects")
    TSubclassOf<UGameplayEffect> OnEquipEffect;

    /**
     * GameplayEffect aplicado al ASC del portador cuando se usa el item.
     * TÃ­picamente Instant o HasDuration para consumibles.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|GameplayEffects")
    TSubclassOf<UGameplayEffect> OnUseEffect;

    // â”€â”€â”€ REQUISITOS Y ECONOMÃA â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Nivel mÃ­nimo del personaje para poder equipar/usar este item */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Requirements", meta = (ClampMin = "1"))
    int32 RequiredLevel = 1;

    /** Valor base del item en monedas del juego (precio de venta base) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Economy", meta = (ClampMin = "0"))
    int32 BaseValue = 0;

    // â”€â”€â”€ MÃ‰TODOS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Determina si este item puede ser usado por un personaje con la clase y raza dadas.
     * Por defecto devuelve true â€” sobreescribir en Blueprint para restricciones especÃ­ficas.
     *
     * @param CharClass  Clase del personaje que intenta usar el item
     * @param Race       Raza del personaje que intenta usar el item
     * @return           true si el personaje puede usar este item
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item|Requirements")
    bool CanBeUsedBy(ESOLOClass CharClass, ESOLORace Race) const;
    virtual bool CanBeUsedBy_Implementation(ESOLOClass CharClass, ESOLORace Race) const;

    /** Devuelve el FPrimaryAssetId usado por el AssetManager para cargar este DataAsset */
    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
