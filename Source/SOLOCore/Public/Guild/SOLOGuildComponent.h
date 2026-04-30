#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Guild/SOLOGuildTypes.h"
#include "SOLOGuildComponent.generated.h"

// â”€â”€â”€ DELEGATES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

/** Disparado cuando los datos del gremio se actualizan (cambio de miembros, banco, MotD, etc.). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGuildDataUpdated,
    const FGuildData&, UpdatedGuildData);

/** Disparado cuando el jugador local ingresa a un gremio. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGuildJoined,
    const FGuildData&, JoinedGuildData);

/** Disparado cuando el jugador local abandona o es expulsado de su gremio. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGuildLeft);

/** Disparado cuando el estado online/offline de un miembro cambia, o su rango es modificado. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMemberStatusChanged,
    const FGuildMember&, ChangedMember);

/**
 * USOLOGuildComponent
 *
 * Componente que va en ASOLOPlayerState.
 * Guarda el estado del gremio del jugador local y expone helpers para
 * consultar rango, permisos y miembros sin tener que acceder directamente
 * a FGuildData. El GuildManager es quien actualiza los datos via UpdateGuildData.
 */
UCLASS(ClassGroup = (SOLO), BlueprintType, Blueprintable,
    meta = (BlueprintSpawnableComponent))
class SOLOCORE_API USOLOGuildComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USOLOGuildComponent();

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // â”€â”€â”€ DELEGATES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Notifica cuando los datos del gremio cambian (cualquier campo). */
    UPROPERTY(BlueprintAssignable, Category = "Guild|Delegates")
    FOnGuildDataUpdated OnGuildDataUpdated;

    /** Notifica cuando el jugador local se une a un gremio. */
    UPROPERTY(BlueprintAssignable, Category = "Guild|Delegates")
    FOnGuildJoined OnGuildJoined;

    /** Notifica cuando el jugador local abandona o es expulsado del gremio. */
    UPROPERTY(BlueprintAssignable, Category = "Guild|Delegates")
    FOnGuildLeft OnGuildLeft;

    /** Notifica cuando el estado de un miembro especifico cambia. */
    UPROPERTY(BlueprintAssignable, Category = "Guild|Delegates")
    FOnMemberStatusChanged OnMemberStatusChanged;

    // â”€â”€â”€ DATOS REPLICADOS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Datos completos del gremio actual. Vacio si el jugador no esta en ningun gremio. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild|Data",
        ReplicatedUsing = OnRep_CurrentGuild)
    FGuildData CurrentGuild;

    /** true si el jugador pertenece a algun gremio. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild|Data",
        ReplicatedUsing = OnRep_bIsInGuild)
    bool bIsInGuild = false;

    /** EOS Product User ID del jugador local (inicializado en BeginPlay desde GameInstance). */
    UPROPERTY(BlueprintReadOnly, Category = "Guild|Data")
    FString LocalPlayerID;

    // â”€â”€â”€ CONSULTAS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Devuelve true si el jugador pertenece a un gremio. */
    UFUNCTION(BlueprintPure, Category = "Guild|Query")
    bool IsInGuild() const;

    /** Devuelve el nombre del gremio actual, o cadena vacia si no esta en uno. */
    UFUNCTION(BlueprintPure, Category = "Guild|Query")
    FString GetGuildName() const;

    /** Devuelve el tag del gremio actual, o cadena vacia si no esta en uno. */
    UFUNCTION(BlueprintPure, Category = "Guild|Query")
    FString GetGuildTag() const;

    /** Devuelve el rango del jugador local dentro de su gremio. */
    UFUNCTION(BlueprintPure, Category = "Guild|Query")
    EGuildRank GetLocalPlayerRank() const;

    /** Devuelve true si el jugador local tiene el permiso solicitado segun su rango. */
    UFUNCTION(BlueprintPure, Category = "Guild|Query")
    bool HasPermission(EGuildPermission Permission) const;

    /**
     * Devuelve un puntero mutable al miembro con el PlayerID dado.
     * Devuelve nullptr si no se encuentra.
     * No expuesto a Blueprint (punteros crudos no son seguros en BP).
     */
    FGuildMember* FindMember(const FString& PlayerID);

    /** Devuelve una copia del array de miembros que estan actualmente online. */
    UFUNCTION(BlueprintCallable, Category = "Guild|Query")
    TArray<FGuildMember> GetOnlineMembers() const;

    // â”€â”€â”€ ACTUALIZACION DE DATOS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Reemplaza los datos del gremio local con los nuevos datos recibidos del GuildManager.
     * Dispara OnGuildDataUpdated. Si antes no estaba en gremio y ahora si, dispara OnGuildJoined.
     */
    UFUNCTION(BlueprintCallable, Category = "Guild|Management")
    void UpdateGuildData(const FGuildData& NewData);

    /**
     * Notifica al GuildManager que el jugador quiere salir del gremio
     * y limpia los datos locales. Dispara OnGuildLeft.
     */
    UFUNCTION(BlueprintCallable, Category = "Guild|Management")
    void LeaveGuild();

protected:
    // â”€â”€â”€ OnRep CALLBACKS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    UFUNCTION()
    void OnRep_CurrentGuild();

    UFUNCTION()
    void OnRep_bIsInGuild();
};
