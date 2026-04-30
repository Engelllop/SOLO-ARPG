#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Guild/SOLOGuildTypes.h"
#include "SOLOGuildManager.generated.h"

class USOLOGuildComponent;
class USOLOInventoryComponent;

// â”€â”€â”€ DELEGATES GLOBALES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

/** Disparado cuando se crea un gremio nuevo en esta sesion. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGuildCreated,
    const FGuildData&, CreatedGuild);

/** Disparado cuando un gremio es disuelto. Pasa el GuildID del gremio eliminado. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGuildDisbanded,
    FString, DisbandedGuildID);

/**
 * USOLOGuildManager
 *
 * GameInstanceSubsystem que centraliza todas las operaciones de gremio.
 * Persiste durante toda la sesion igual que la GameInstance.
 * En modo single-player actua como autoridad local; la arquitectura
 * esta preparada para delegar operaciones al servidor en modo online.
 *
 * Uso:
 *   USOLOGuildManager* GuildMgr =
 *       GetGameInstance()->GetSubsystem<USOLOGuildManager>();
 */
UCLASS()
class SOLOCORE_API USOLOGuildManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // â”€â”€â”€ DELEGATES â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /** Notifica a observers cuando se crea un gremio nuevo. */
    UPROPERTY(BlueprintAssignable, Category = "Guild|Delegates")
    FOnGuildCreated OnGuildCreated;

    /** Notifica a observers cuando un gremio es disuelto. */
    UPROPERTY(BlueprintAssignable, Category = "Guild|Delegates")
    FOnGuildDisbanded OnGuildDisbanded;

    // â”€â”€â”€ DATOS DE SESION â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Mapa de todos los gremios cargados en esta sesion, indexados por GuildID.
     * En produccion online estos datos vendrÃ­an de un backend persistente.
     */
    UPROPERTY(BlueprintReadOnly, Category = "Guild|Data")
    TMap<FString, FGuildData> ActiveGuilds;

    /** GuildID del gremio al que pertenece el jugador local. Vacio si no tiene gremio. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild|Data")
    FString LocalPlayerGuildID;

    // â”€â”€â”€ OPERACIONES DE GREMIO â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Crea un gremio nuevo con el fundador como GuildMaster.
     *
     * Validaciones:
     * - FounderComp no puede ser nullptr.
     * - El fundador no puede estar ya en un gremio.
     * - GuildName debe ser unico entre los gremios activos.
     * - GuildTag debe tener entre 2 y 5 caracteres.
     *
     * Si tiene exito actualiza FounderComp con los nuevos datos y dispara OnGuildCreated.
     *
     * @param GuildName   Nombre completo del gremio.
     * @param GuildTag    Tag corto (2-5 chars).
     * @param FounderComp Componente de gremio del jugador que funda el gremio.
     */
    UFUNCTION(BlueprintCallable, Category = "Guild|Operations")
    EGuildRequestResult CreateGuild(const FString& GuildName,
                                    const FString& GuildTag,
                                    USOLOGuildComponent* FounderComp);

    /**
     * Invita a un jugador al gremio. El invitador debe tener permiso InviteMembers.
     * En la implementacion actual anade directamente al jugador (sin flujo de aceptacion).
     *
     * @param InviterID        PlayerID del miembro que invita.
     * @param TargetPlayerName Nombre del jugador a invitar (se busca entre componentes activos).
     * @param GuildID          ID del gremio al que se invita.
     */
    UFUNCTION(BlueprintCallable, Category = "Guild|Operations")
    EGuildRequestResult InvitePlayer(const FString& InviterID,
                                     const FString& TargetPlayerName,
                                     const FString& GuildID);

    /**
     * Expulsa a un miembro del gremio. El ejecutor debe tener permiso KickMembers
     * y no puede expulsar a alguien de igual o mayor rango.
     *
     * @param KickerID  PlayerID del miembro que expulsa.
     * @param TargetID  PlayerID del miembro a expulsar.
     * @param GuildID   ID del gremio.
     */
    UFUNCTION(BlueprintCallable, Category = "Guild|Operations")
    EGuildRequestResult KickMember(const FString& KickerID,
                                   const FString& TargetID,
                                   const FString& GuildID);

    /**
     * Asciende a un miembro un rango. El ejecutor debe tener permiso PromoteMembers
     * y no puede ascender a alguien a su mismo rango o superior.
     *
     * @param PromoterID PlayerID del miembro que asciende.
     * @param TargetID   PlayerID del miembro a ascender.
     * @param GuildID    ID del gremio.
     */
    UFUNCTION(BlueprintCallable, Category = "Guild|Operations")
    EGuildRequestResult PromoteMember(const FString& PromoterID,
                                      const FString& TargetID,
                                      const FString& GuildID);

    /**
     * Desciende a un miembro un rango. El ejecutor debe tener permiso PromoteMembers
     * y no puede descender a alguien de igual o mayor rango.
     *
     * @param DemoterID PlayerID del miembro que desciende.
     * @param TargetID  PlayerID del miembro a descender.
     * @param GuildID   ID del gremio.
     */
    UFUNCTION(BlueprintCallable, Category = "Guild|Operations")
    EGuildRequestResult DemoteMember(const FString& DemoterID,
                                     const FString& TargetID,
                                     const FString& GuildID);

    /**
     * Disuelve el gremio. Solo puede hacerlo el GuildMaster.
     * Elimina el gremio de ActiveGuilds y dispara OnGuildDisbanded.
     *
     * @param RequestingPlayerID PlayerID del GuildMaster.
     * @param GuildID            ID del gremio a disolver.
     */
    UFUNCTION(BlueprintCallable, Category = "Guild|Operations")
    EGuildRequestResult DisbandGuild(const FString& RequestingPlayerID,
                                     const FString& GuildID);

    /**
     * Deposita moneda del inventario del jugador al banco del gremio.
     * Cualquier miembro puede depositar (no requiere ManageBank).
     * Incrementa GuildContribution del miembro y GuildXP del gremio.
     *
     * @param PlayerID        PlayerID del depositante.
     * @param GuildID         ID del gremio.
     * @param Amount          Cantidad a depositar.
     * @param PlayerInventory Inventario del jugador (origen de los fondos).
     */
    UFUNCTION(BlueprintCallable, Category = "Guild|Bank")
    EGuildRequestResult DepositToBank(const FString& PlayerID,
                                      const FString& GuildID,
                                      int32 Amount,
                                      USOLOInventoryComponent* PlayerInventory);

    /**
     * Retira moneda del banco del gremio al inventario del jugador.
     * Requiere permiso ManageBank.
     *
     * @param PlayerID        PlayerID del solicitante.
     * @param GuildID         ID del gremio.
     * @param Amount          Cantidad a retirar.
     * @param PlayerInventory Inventario del jugador (destino de los fondos).
     */
    UFUNCTION(BlueprintCallable, Category = "Guild|Bank")
    EGuildRequestResult WithdrawFromBank(const FString& PlayerID,
                                         const FString& GuildID,
                                         int32 Amount,
                                         USOLOInventoryComponent* PlayerInventory);

    /**
     * Actualiza el Mensaje del Dia del gremio. Requiere permiso EditMotD.
     *
     * @param PlayerID  PlayerID del miembro que edita.
     * @param GuildID   ID del gremio.
     * @param NewMotD   Nuevo texto del mensaje.
     */
    UFUNCTION(BlueprintCallable, Category = "Guild|Operations")
    EGuildRequestResult UpdateMotD(const FString& PlayerID,
                                   const FString& GuildID,
                                   const FString& NewMotD);

    // â”€â”€â”€ BUSQUEDA â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Devuelve un puntero mutable al FGuildData con el GuildID dado,
     * o nullptr si no existe en esta sesion.
     */
    FGuildData* FindGuild(const FString& GuildID);

    /**
     * Devuelve un puntero mutable al FGuildData con el nombre dado (case-insensitive),
     * o nullptr si no existe.
     */
    FGuildData* FindGuildByName(const FString& GuildName);

    // â”€â”€â”€ PROGRESION â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Agrega XP al gremio y sube de nivel si se alcanza el threshold.
     * Threshold por nivel: 500 * Level^1.5  (acumulado desde nivel actual).
     * El nivel maximo es 10.
     *
     * @param GuildID  ID del gremio que recibe el XP.
     * @param XPAmount Cantidad de XP a agregar.
     */
    UFUNCTION(BlueprintCallable, Category = "Guild|Progression")
    void AddGuildXP(const FString& GuildID, int32 XPAmount);

private:
    // â”€â”€â”€ HELPERS PRIVADOS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

    /**
     * Busca y devuelve un puntero mutable al FGuildMember con el PlayerID dado
     * dentro del gremio especificado. Devuelve nullptr si no se encuentra.
     */
    FGuildMember* FindMemberInGuild(FGuildData& Guild, const FString& PlayerID);

    /**
     * Devuelve true si el miembro con ActorID tiene el permiso requerido
     * dentro del gremio especificado.
     */
    bool CanPerformAction(const FGuildData& Guild,
                          const FString& ActorID,
                          EGuildPermission RequiredPermission) const;

    /**
     * Propaga los datos actualizados del gremio a todos los GuildComponents
     * activos en el mundo cuyo LocalPlayerID pertenezca al gremio.
     */
    void NotifyGuildMembersUpdated(const FString& GuildID);
};
