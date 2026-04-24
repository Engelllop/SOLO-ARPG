#include "Guild/EMBERVEILGuildManager.h"
#include "Guild/EMBERVEILGuildComponent.h"
#include "Inventory/EMBERVEILInventoryComponent.h"
#include "GameFramework/EMBERVEILGameInstance.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"
#include "Misc/Guid.h"
#include "Math/UnrealMathUtility.h"

// ─── HasGuildPermission (implementacion global definida en Types) ─────────────

bool HasGuildPermission(EGuildRank Rank, EGuildPermission Permission)
{
    switch (Rank)
    {
    case EGuildRank::GuildMaster:
        // El GuildMaster tiene todos los permisos
        return true;

    case EGuildRank::Officer:
        // El Oficial tiene todos los permisos excepto DeclareWar y ManageGuildHouse
        return (Permission != EGuildPermission::DeclareWar &&
                Permission != EGuildPermission::ManageGuildHouse);

    case EGuildRank::Veteran:
        // El Veterano solo puede invitar y editar el MotD
        return (Permission == EGuildPermission::InviteMembers ||
                Permission == EGuildPermission::EditMotD);

    case EGuildRank::Member:
    case EGuildRank::Recruit:
    default:
        // Miembro y Recluta no tienen permisos especiales
        return false;
    }
}

// ─── HELPERS PRIVADOS ─────────────────────────────────────────────────────────

FGuildMember* UEMBERVEILGuildManager::FindMemberInGuild(FGuildData& Guild,
                                                          const FString& PlayerID)
{
    for (FGuildMember& Member : Guild.Members)
    {
        if (Member.PlayerID == PlayerID)
        {
            return &Member;
        }
    }
    return nullptr;
}

bool UEMBERVEILGuildManager::CanPerformAction(const FGuildData& Guild,
                                               const FString& ActorID,
                                               EGuildPermission RequiredPermission) const
{
    for (const FGuildMember& Member : Guild.Members)
    {
        if (Member.PlayerID == ActorID)
        {
            return HasGuildPermission(Member.Rank, RequiredPermission);
        }
    }
    return false;
}

void UEMBERVEILGuildManager::NotifyGuildMembersUpdated(const FString& GuildID)
{
    FGuildData* Guild = FindGuild(GuildID);
    if (!Guild)
    {
        return;
    }

    UWorld* World = GetGameInstance()->GetWorld();
    if (!World)
    {
        return;
    }

    // Iterar sobre todos los PlayerState activos y notificar a los que pertenezcan al gremio
    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (!PC)
        {
            continue;
        }

        APlayerState* PS = PC->GetPlayerState<APlayerState>();
        if (!PS)
        {
            continue;
        }

        UEMBERVEILGuildComponent* GuildComp =
            PS->FindComponentByClass<UEMBERVEILGuildComponent>();

        if (!GuildComp)
        {
            continue;
        }

        // Solo actualizar si este jugador es miembro del gremio modificado
        if (FindMemberInGuild(*Guild, GuildComp->LocalPlayerID) != nullptr)
        {
            GuildComp->UpdateGuildData(*Guild);
        }
    }
}

// ─── BUSQUEDA ─────────────────────────────────────────────────────────────────

FGuildData* UEMBERVEILGuildManager::FindGuild(const FString& GuildID)
{
    return ActiveGuilds.Find(GuildID);
}

FGuildData* UEMBERVEILGuildManager::FindGuildByName(const FString& GuildName)
{
    for (TPair<FString, FGuildData>& Pair : ActiveGuilds)
    {
        if (Pair.Value.GuildName.Equals(GuildName, ESearchCase::IgnoreCase))
        {
            return &Pair.Value;
        }
    }
    return nullptr;
}

// ─── CREACION DE GREMIO ───────────────────────────────────────────────────────

EGuildRequestResult UEMBERVEILGuildManager::CreateGuild(const FString& GuildName,
                                                         const FString& GuildTag,
                                                         UEMBERVEILGuildComponent* FounderComp)
{
    if (!FounderComp)
    {
        return EGuildRequestResult::Failed_PlayerNotFound;
    }

    // El fundador no puede estar ya en un gremio
    if (FounderComp->IsInGuild())
    {
        return EGuildRequestResult::Failed_AlreadyInGuild;
    }

    // Validar el tag (2-5 caracteres)
    const int32 TagLen = GuildTag.Len();
    if (TagLen < 2 || TagLen > 5)
    {
        return EGuildRequestResult::Failed_NameTaken; // reutilizamos el resultado mas cercano
    }

    // Nombre debe ser unico
    if (FindGuildByName(GuildName) != nullptr)
    {
        return EGuildRequestResult::Failed_NameTaken;
    }

    // Construir datos del nuevo gremio
    FGuildData NewGuild;
    NewGuild.GuildID      = FGuid::NewGuid().ToString();
    NewGuild.GuildName    = GuildName;
    NewGuild.GuildTag     = GuildTag;
    NewGuild.FoundedDate  = FDateTime::UtcNow();
    NewGuild.GuildLevel   = 1;
    NewGuild.GuildXP      = 0;
    NewGuild.MaxMembers   = 50;

    // Construir el miembro fundador como GuildMaster
    FGuildMember Founder;
    Founder.PlayerID    = FounderComp->LocalPlayerID;
    Founder.PlayerName  = FounderComp->LocalPlayerID; // se puede actualizar con datos del PS
    Founder.Rank        = EGuildRank::GuildMaster;
    Founder.bIsOnline   = true;
    Founder.LastSeen    = FDateTime::UtcNow();

    // Tomar clase y nivel del PlayerSaveData si la GameInstance esta disponible
    UEMBERVEILGameInstance* GI = Cast<UEMBERVEILGameInstance>(GetGameInstance());
    if (GI)
    {
        Founder.PlayerClass = GI->ActivePlayerData.Class;
        Founder.PlayerLevel = GI->ActivePlayerData.Level;
        Founder.PlayerName  = GI->ActivePlayerData.CharacterName.IsEmpty()
                                   ? FounderComp->LocalPlayerID
                                   : GI->ActivePlayerData.CharacterName;
    }

    NewGuild.Members.Add(Founder);

    // Persistir en ActiveGuilds
    ActiveGuilds.Add(NewGuild.GuildID, NewGuild);
    LocalPlayerGuildID = NewGuild.GuildID;

    // Notificar al componente del fundador
    FounderComp->UpdateGuildData(NewGuild);

    // Disparar delegate global
    OnGuildCreated.Broadcast(NewGuild);

    return EGuildRequestResult::Success;
}

// ─── INVITACION ───────────────────────────────────────────────────────────────

EGuildRequestResult UEMBERVEILGuildManager::InvitePlayer(const FString& InviterID,
                                                          const FString& TargetPlayerName,
                                                          const FString& GuildID)
{
    FGuildData* Guild = FindGuild(GuildID);
    if (!Guild)
    {
        return EGuildRequestResult::Failed_GuildNotFound;
    }

    if (!CanPerformAction(*Guild, InviterID, EGuildPermission::InviteMembers))
    {
        return EGuildRequestResult::Failed_InsufficientRank;
    }

    if (Guild->IsFull())
    {
        return EGuildRequestResult::Failed_GuildFull;
    }

    // Buscar el GuildComponent del jugador objetivo por nombre de personaje
    UWorld* World = GetGameInstance()->GetWorld();
    if (!World)
    {
        return EGuildRequestResult::Failed_PlayerNotFound;
    }

    UEMBERVEILGuildComponent* TargetComp = nullptr;

    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (!PC)
        {
            continue;
        }

        APlayerState* PS = PC->GetPlayerState<APlayerState>();
        if (!PS)
        {
            continue;
        }

        UEMBERVEILGuildComponent* GuildComp =
            PS->FindComponentByClass<UEMBERVEILGuildComponent>();

        if (!GuildComp)
        {
            continue;
        }

        // Comparar nombre del personaje via GameInstance
        UEMBERVEILGameInstance* GI = Cast<UEMBERVEILGameInstance>(GetGameInstance());
        if (GI && GI->ActivePlayerData.CharacterName.Equals(TargetPlayerName,
                                                              ESearchCase::IgnoreCase))
        {
            TargetComp = GuildComp;
            break;
        }
    }

    if (!TargetComp)
    {
        return EGuildRequestResult::Failed_PlayerNotFound;
    }

    if (TargetComp->IsInGuild())
    {
        return EGuildRequestResult::Failed_AlreadyInGuild;
    }

    // Crear entrada del nuevo miembro
    FGuildMember NewMember;
    NewMember.PlayerID   = TargetComp->LocalPlayerID;
    NewMember.Rank       = EGuildRank::Recruit;
    NewMember.bIsOnline  = true;
    NewMember.LastSeen   = FDateTime::UtcNow();

    UEMBERVEILGameInstance* GI = Cast<UEMBERVEILGameInstance>(GetGameInstance());
    if (GI)
    {
        NewMember.PlayerClass = GI->ActivePlayerData.Class;
        NewMember.PlayerLevel = GI->ActivePlayerData.Level;
        NewMember.PlayerName  = GI->ActivePlayerData.CharacterName.IsEmpty()
                                     ? TargetComp->LocalPlayerID
                                     : GI->ActivePlayerData.CharacterName;
    }
    else
    {
        NewMember.PlayerName = TargetComp->LocalPlayerID;
    }

    Guild->Members.Add(NewMember);
    TargetComp->UpdateGuildData(*Guild);
    NotifyGuildMembersUpdated(GuildID);

    return EGuildRequestResult::Success;
}

// ─── EXPULSION ────────────────────────────────────────────────────────────────

EGuildRequestResult UEMBERVEILGuildManager::KickMember(const FString& KickerID,
                                                        const FString& TargetID,
                                                        const FString& GuildID)
{
    FGuildData* Guild = FindGuild(GuildID);
    if (!Guild)
    {
        return EGuildRequestResult::Failed_GuildNotFound;
    }

    // Un jugador siempre puede "expulsarse" a si mismo (salir del gremio)
    const bool bSelfLeave = (KickerID == TargetID);

    if (!bSelfLeave && !CanPerformAction(*Guild, KickerID, EGuildPermission::KickMembers))
    {
        return EGuildRequestResult::Failed_InsufficientRank;
    }

    FGuildMember* KickerMember = FindMemberInGuild(*Guild, KickerID);
    FGuildMember* TargetMember = FindMemberInGuild(*Guild, TargetID);

    if (!TargetMember)
    {
        return EGuildRequestResult::Failed_PlayerNotFound;
    }

    // No se puede expulsar a alguien de igual o mayor rango (excepto si es uno mismo)
    if (!bSelfLeave && KickerMember)
    {
        if (static_cast<uint8>(TargetMember->Rank) >= static_cast<uint8>(KickerMember->Rank))
        {
            return EGuildRequestResult::Failed_InsufficientRank;
        }
    }

    // Eliminar al miembro del array
    Guild->Members.RemoveAll([&TargetID](const FGuildMember& M)
    {
        return M.PlayerID == TargetID;
    });

    // Notificar a los demas miembros
    NotifyGuildMembersUpdated(GuildID);

    return EGuildRequestResult::Success;
}

// ─── ASCENSO ──────────────────────────────────────────────────────────────────

EGuildRequestResult UEMBERVEILGuildManager::PromoteMember(const FString& PromoterID,
                                                           const FString& TargetID,
                                                           const FString& GuildID)
{
    FGuildData* Guild = FindGuild(GuildID);
    if (!Guild)
    {
        return EGuildRequestResult::Failed_GuildNotFound;
    }

    if (!CanPerformAction(*Guild, PromoterID, EGuildPermission::PromoteMembers))
    {
        return EGuildRequestResult::Failed_InsufficientRank;
    }

    FGuildMember* PromoterMember = FindMemberInGuild(*Guild, PromoterID);
    FGuildMember* TargetMember   = FindMemberInGuild(*Guild, TargetID);

    if (!TargetMember)
    {
        return EGuildRequestResult::Failed_PlayerNotFound;
    }

    if (!PromoterMember)
    {
        return EGuildRequestResult::Failed_PlayerNotFound;
    }

    // El objetivo ya es GuildMaster o su rango seria igual/superior al del promotor
    const uint8 TargetRankVal   = static_cast<uint8>(TargetMember->Rank);
    const uint8 PromoterRankVal = static_cast<uint8>(PromoterMember->Rank);

    // El nuevo rango del objetivo seria TargetRank + 1; no puede ser igual o mayor al promotor
    if (TargetRankVal + 1 >= PromoterRankVal)
    {
        return EGuildRequestResult::Failed_InsufficientRank;
    }

    TargetMember->Rank = static_cast<EGuildRank>(TargetRankVal + 1);
    NotifyGuildMembersUpdated(GuildID);

    return EGuildRequestResult::Success;
}

// ─── DESCENSO ─────────────────────────────────────────────────────────────────

EGuildRequestResult UEMBERVEILGuildManager::DemoteMember(const FString& DemoterID,
                                                          const FString& TargetID,
                                                          const FString& GuildID)
{
    FGuildData* Guild = FindGuild(GuildID);
    if (!Guild)
    {
        return EGuildRequestResult::Failed_GuildNotFound;
    }

    if (!CanPerformAction(*Guild, DemoterID, EGuildPermission::PromoteMembers))
    {
        return EGuildRequestResult::Failed_InsufficientRank;
    }

    FGuildMember* DemoterMember = FindMemberInGuild(*Guild, DemoterID);
    FGuildMember* TargetMember  = FindMemberInGuild(*Guild, TargetID);

    if (!TargetMember)
    {
        return EGuildRequestResult::Failed_PlayerNotFound;
    }

    if (!DemoterMember)
    {
        return EGuildRequestResult::Failed_PlayerNotFound;
    }

    // No se puede descender a alguien de igual o mayor rango
    if (static_cast<uint8>(TargetMember->Rank) >= static_cast<uint8>(DemoterMember->Rank))
    {
        return EGuildRequestResult::Failed_InsufficientRank;
    }

    // No se puede descender mas alla de Recruit
    if (TargetMember->Rank == EGuildRank::Recruit)
    {
        return EGuildRequestResult::Failed_InsufficientRank;
    }

    const uint8 TargetRankVal = static_cast<uint8>(TargetMember->Rank);
    TargetMember->Rank = static_cast<EGuildRank>(TargetRankVal - 1);
    NotifyGuildMembersUpdated(GuildID);

    return EGuildRequestResult::Success;
}

// ─── DISOLUCION ───────────────────────────────────────────────────────────────

EGuildRequestResult UEMBERVEILGuildManager::DisbandGuild(const FString& RequestingPlayerID,
                                                          const FString& GuildID)
{
    FGuildData* Guild = FindGuild(GuildID);
    if (!Guild)
    {
        return EGuildRequestResult::Failed_GuildNotFound;
    }

    // Solo el GuildMaster puede disolver el gremio
    const FGuildMember* Requester = FindMemberInGuild(*Guild, RequestingPlayerID);
    if (!Requester || Requester->Rank != EGuildRank::GuildMaster)
    {
        return EGuildRequestResult::Failed_InsufficientRank;
    }

    // Notificar a todos los miembros activos antes de eliminar los datos
    UWorld* World = GetGameInstance()->GetWorld();
    if (World)
    {
        for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
        {
            APlayerController* PC = It->Get();
            if (!PC)
            {
                continue;
            }

            APlayerState* PS = PC->GetPlayerState<APlayerState>();
            if (!PS)
            {
                continue;
            }

            UEMBERVEILGuildComponent* GuildComp =
                PS->FindComponentByClass<UEMBERVEILGuildComponent>();

            if (GuildComp && GuildComp->IsInGuild() &&
                GuildComp->CurrentGuild.GuildID == GuildID)
            {
                // Limpiar datos del componente y disparar OnGuildLeft
                GuildComp->UpdateGuildData(FGuildData());
                GuildComp->OnGuildLeft.Broadcast();
            }
        }
    }

    if (LocalPlayerGuildID == GuildID)
    {
        LocalPlayerGuildID.Empty();
    }

    ActiveGuilds.Remove(GuildID);
    OnGuildDisbanded.Broadcast(GuildID);

    return EGuildRequestResult::Success;
}

// ─── BANCO DEL GREMIO ─────────────────────────────────────────────────────────

EGuildRequestResult UEMBERVEILGuildManager::DepositToBank(
    const FString& PlayerID,
    const FString& GuildID,
    int32 Amount,
    UEMBERVEILInventoryComponent* PlayerInventory)
{
    if (!PlayerInventory)
    {
        return EGuildRequestResult::Failed_PlayerNotFound;
    }

    if (Amount <= 0)
    {
        return EGuildRequestResult::Failed_InsufficientFunds;
    }

    FGuildData* Guild = FindGuild(GuildID);
    if (!Guild)
    {
        return EGuildRequestResult::Failed_GuildNotFound;
    }

    FGuildMember* Member = FindMemberInGuild(*Guild, PlayerID);
    if (!Member)
    {
        return EGuildRequestResult::Failed_PlayerNotFound;
    }

    // Intentar gastar la moneda del inventario del jugador
    if (!PlayerInventory->SpendCurrency(Amount))
    {
        return EGuildRequestResult::Failed_InsufficientFunds;
    }

    Guild->GuildBankCurrency    += Amount;
    Member->GuildContribution   += Amount;

    // Las contribuciones otorgan XP al gremio (1 XP por cada unidad de moneda)
    AddGuildXP(GuildID, Amount);

    NotifyGuildMembersUpdated(GuildID);

    return EGuildRequestResult::Success;
}

EGuildRequestResult UEMBERVEILGuildManager::WithdrawFromBank(
    const FString& PlayerID,
    const FString& GuildID,
    int32 Amount,
    UEMBERVEILInventoryComponent* PlayerInventory)
{
    if (!PlayerInventory)
    {
        return EGuildRequestResult::Failed_PlayerNotFound;
    }

    if (Amount <= 0)
    {
        return EGuildRequestResult::Failed_InsufficientFunds;
    }

    FGuildData* Guild = FindGuild(GuildID);
    if (!Guild)
    {
        return EGuildRequestResult::Failed_GuildNotFound;
    }

    if (!CanPerformAction(*Guild, PlayerID, EGuildPermission::ManageBank))
    {
        return EGuildRequestResult::Failed_InsufficientRank;
    }

    if (Guild->GuildBankCurrency < Amount)
    {
        return EGuildRequestResult::Failed_InsufficientFunds;
    }

    Guild->GuildBankCurrency -= Amount;
    PlayerInventory->AddCurrency(Amount);

    NotifyGuildMembersUpdated(GuildID);

    return EGuildRequestResult::Success;
}

// ─── MENSAJE DEL DIA ──────────────────────────────────────────────────────────

EGuildRequestResult UEMBERVEILGuildManager::UpdateMotD(const FString& PlayerID,
                                                        const FString& GuildID,
                                                        const FString& NewMotD)
{
    FGuildData* Guild = FindGuild(GuildID);
    if (!Guild)
    {
        return EGuildRequestResult::Failed_GuildNotFound;
    }

    if (!CanPerformAction(*Guild, PlayerID, EGuildPermission::EditMotD))
    {
        return EGuildRequestResult::Failed_InsufficientRank;
    }

    Guild->MotD = NewMotD;
    NotifyGuildMembersUpdated(GuildID);

    return EGuildRequestResult::Success;
}

// ─── PROGRESION ───────────────────────────────────────────────────────────────

void UEMBERVEILGuildManager::AddGuildXP(const FString& GuildID, int32 XPAmount)
{
    FGuildData* Guild = FindGuild(GuildID);
    if (!Guild || XPAmount <= 0)
    {
        return;
    }

    const int32 MaxGuildLevel = 10;
    if (Guild->GuildLevel >= MaxGuildLevel)
    {
        return;
    }

    Guild->GuildXP += XPAmount;

    // Subir de nivel mientras se supere el threshold: 500 * Level^1.5
    bool bLeveledUp = false;
    while (Guild->GuildLevel < MaxGuildLevel)
    {
        const float LevelFloat   = static_cast<float>(Guild->GuildLevel);
        const int32 XPThreshold  = FMath::RoundToInt(500.f * FMath::Pow(LevelFloat, 1.5f));

        if (Guild->GuildXP < XPThreshold)
        {
            break;
        }

        Guild->GuildXP   -= XPThreshold;
        Guild->GuildLevel += 1;
        bLeveledUp         = true;
    }

    if (bLeveledUp)
    {
        NotifyGuildMembersUpdated(GuildID);
    }
}
