#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "GameFramework/EMBERVEILGameInstance.h"
#include "EMBERVEILGuildTypes.generated.h"

// ─── ENUMS ────────────────────────────────────────────────────────────────────

UENUM(BlueprintType)
enum class EGuildRank : uint8
{
    Recruit     UMETA(DisplayName = "Recluta"),
    Member      UMETA(DisplayName = "Miembro"),
    Veteran     UMETA(DisplayName = "Veterano"),
    Officer     UMETA(DisplayName = "Oficial"),
    GuildMaster UMETA(DisplayName = "Maestro del Gremio")
};

UENUM(BlueprintType)
enum class EGuildPermission : uint8
{
    InviteMembers       UMETA(DisplayName = "Invitar Miembros"),
    KickMembers         UMETA(DisplayName = "Expulsar Miembros"),
    PromoteMembers      UMETA(DisplayName = "Ascender Miembros"),
    ManageBank          UMETA(DisplayName = "Gestionar Banco"),
    ManageGuildHouse    UMETA(DisplayName = "Gestionar Casa del Gremio"),
    DeclareWar          UMETA(DisplayName = "Declarar Guerra"),      // para futuro PvP
    EditMotD            UMETA(DisplayName = "Editar Mensaje del Dia") // Message of the Day
};

UENUM(BlueprintType)
enum class EGuildRequestResult : uint8
{
    Success                     UMETA(DisplayName = "Exito"),
    Failed_AlreadyInGuild       UMETA(DisplayName = "Ya esta en un Gremio"),
    Failed_GuildFull            UMETA(DisplayName = "Gremio Lleno"),
    Failed_InsufficientRank     UMETA(DisplayName = "Rango Insuficiente"),
    Failed_GuildNotFound        UMETA(DisplayName = "Gremio No Encontrado"),
    Failed_PlayerNotFound       UMETA(DisplayName = "Jugador No Encontrado"),
    Failed_InsufficientFunds    UMETA(DisplayName = "Fondos Insuficientes"),
    Failed_NameTaken            UMETA(DisplayName = "Nombre Ocupado")
};

// ─── STRUCTS ──────────────────────────────────────────────────────────────────

/** Representa a un miembro del gremio con su estado e informacion de personaje. */
USTRUCT(BlueprintType)
struct EMBERVEILCORE_API FGuildMember
{
    GENERATED_BODY()

    /** Nombre visible del personaje. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild|Member")
    FString PlayerName;

    /** EOS Product User ID serializado como string. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild|Member")
    FString PlayerID;

    /** Rango actual dentro del gremio. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild|Member")
    EGuildRank Rank = EGuildRank::Recruit;

    /** Clase de personaje del miembro. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild|Member")
    EEMBERVEILClass PlayerClass = EEMBERVEILClass::Warrior;

    /** Nivel actual del personaje. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild|Member")
    int32 PlayerLevel = 1;

    /** true si el jugador esta conectado en este momento. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild|Member")
    bool bIsOnline = false;

    /** Ultima vez que el jugador estuvo activo. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild|Member")
    FDateTime LastSeen;

    /** Suma total de moneda depositada al banco del gremio por este miembro. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild|Member")
    int32 GuildContribution = 0;
};

/** Datos completos de un gremio. */
USTRUCT(BlueprintType)
struct EMBERVEILCORE_API FGuildData
{
    GENERATED_BODY()

    /** GUID unico generado al crear el gremio. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild")
    FString GuildID;

    /** Nombre completo del gremio. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild")
    FString GuildName;

    /** Tag corto entre 2 y 5 caracteres que aparece junto al nombre. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild")
    FString GuildTag;

    /** Mensaje del dia visible para todos los miembros al iniciar sesion. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild")
    FString MotD;

    /** Descripcion publica del gremio. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild")
    FString Description;

    /** Numero maximo de miembros permitidos (crece con el nivel del gremio). */
    UPROPERTY(BlueprintReadOnly, Category = "Guild")
    int32 MaxMembers = 50;

    /** Lista de todos los miembros actuales. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild")
    TArray<FGuildMember> Members;

    /** Moneda almacenada en el banco del gremio. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild")
    int32 GuildBankCurrency = 0;

    /** Nivel del gremio (1-10). Sube acumulando GuildXP. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild")
    int32 GuildLevel = 1;

    /** XP acumulado hacia el siguiente nivel. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild")
    int32 GuildXP = 0;

    /** Fecha y hora en que fue fundado el gremio. */
    UPROPERTY(BlueprintReadOnly, Category = "Guild")
    FDateTime FoundedDate;

    // ─── Helpers inline ───────────────────────────────────────────────────────

    /** Devuelve true si el GuildID no esta vacio (datos validos). */
    bool IsValid() const { return !GuildID.IsEmpty(); }

    /** Numero actual de miembros. */
    int32 GetMemberCount() const { return Members.Num(); }

    /** true si el gremio ha alcanzado su capacidad maxima. */
    bool IsFull() const { return Members.Num() >= MaxMembers; }
};

// ─── PERMISSION HELPER ────────────────────────────────────────────────────────

/**
 * Devuelve true si el rango dado tiene la permission solicitada.
 *
 * GuildMaster : todo.
 * Officer     : todo excepto DeclareWar y ManageGuildHouse.
 * Veteran     : InviteMembers, EditMotD.
 * Member      : ninguno (puede depositar al banco sin flag propio).
 * Recruit     : ninguno.
 */
EMBERVEILCORE_API bool HasGuildPermission(EGuildRank Rank, EGuildPermission Permission);
