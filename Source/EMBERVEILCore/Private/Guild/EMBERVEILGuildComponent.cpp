#include "Guild/EMBERVEILGuildComponent.h"
#include "Guild/EMBERVEILGuildManager.h"
#include "GameFramework/EMBERVEILGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

// ─── CONSTRUCTOR ──────────────────────────────────────────────────────────────

UEMBERVEILGuildComponent::UEMBERVEILGuildComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

// ─── LIFECYCLE ────────────────────────────────────────────────────────────────

void UEMBERVEILGuildComponent::BeginPlay()
{
    Super::BeginPlay();

    // Inicializar LocalPlayerID desde la GameInstance (EOS Product User ID)
    UEMBERVEILGameInstance* GI = GetWorld()
        ? GetWorld()->GetGameInstance<UEMBERVEILGameInstance>()
        : nullptr;

    if (GI)
    {
        LocalPlayerID = GI->GetLocalUserId();
    }
}

void UEMBERVEILGuildComponent::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UEMBERVEILGuildComponent, CurrentGuild);
    DOREPLIFETIME(UEMBERVEILGuildComponent, bIsInGuild);
}

// ─── CONSULTAS ────────────────────────────────────────────────────────────────

bool UEMBERVEILGuildComponent::IsInGuild() const
{
    return bIsInGuild && CurrentGuild.IsValid();
}

FString UEMBERVEILGuildComponent::GetGuildName() const
{
    return IsInGuild() ? CurrentGuild.GuildName : FString();
}

FString UEMBERVEILGuildComponent::GetGuildTag() const
{
    return IsInGuild() ? CurrentGuild.GuildTag : FString();
}

EGuildRank UEMBERVEILGuildComponent::GetLocalPlayerRank() const
{
    if (!IsInGuild())
    {
        return EGuildRank::Recruit;
    }

    for (const FGuildMember& Member : CurrentGuild.Members)
    {
        if (Member.PlayerID == LocalPlayerID)
        {
            return Member.Rank;
        }
    }

    return EGuildRank::Recruit;
}

bool UEMBERVEILGuildComponent::HasPermission(EGuildPermission Permission) const
{
    return HasGuildPermission(GetLocalPlayerRank(), Permission);
}

FGuildMember* UEMBERVEILGuildComponent::FindMember(const FString& PlayerID)
{
    for (FGuildMember& Member : CurrentGuild.Members)
    {
        if (Member.PlayerID == PlayerID)
        {
            return &Member;
        }
    }
    return nullptr;
}

TArray<FGuildMember> UEMBERVEILGuildComponent::GetOnlineMembers() const
{
    TArray<FGuildMember> OnlineMembers;
    for (const FGuildMember& Member : CurrentGuild.Members)
    {
        if (Member.bIsOnline)
        {
            OnlineMembers.Add(Member);
        }
    }
    return OnlineMembers;
}

// ─── ACTUALIZACION DE DATOS ───────────────────────────────────────────────────

void UEMBERVEILGuildComponent::UpdateGuildData(const FGuildData& NewData)
{
    const bool bWasInGuild = bIsInGuild;

    CurrentGuild = NewData;
    bIsInGuild   = NewData.IsValid();

    if (!bWasInGuild && bIsInGuild)
    {
        OnGuildJoined.Broadcast(CurrentGuild);
    }

    OnGuildDataUpdated.Broadcast(CurrentGuild);
}

void UEMBERVEILGuildComponent::LeaveGuild()
{
    if (!IsInGuild())
    {
        return;
    }

    // Notificar al GuildManager antes de limpiar los datos locales
    UEMBERVEILGuildManager* GuildManager = GetWorld()
        ? GetWorld()->GetGameInstance()->GetSubsystem<UEMBERVEILGuildManager>()
        : nullptr;

    if (GuildManager)
    {
        // Expulsamos al propio jugador usando KickMember con su propio ID
        // (el manager valida rangos, pero el propio jugador siempre puede irse)
        GuildManager->KickMember(LocalPlayerID, LocalPlayerID, CurrentGuild.GuildID);
    }

    // Limpiar datos locales independientemente del resultado del manager
    CurrentGuild  = FGuildData();
    bIsInGuild    = false;

    OnGuildLeft.Broadcast();
}

// ─── OnRep CALLBACKS ──────────────────────────────────────────────────────────

void UEMBERVEILGuildComponent::OnRep_CurrentGuild()
{
    OnGuildDataUpdated.Broadcast(CurrentGuild);
}

void UEMBERVEILGuildComponent::OnRep_bIsInGuild()
{
    if (!bIsInGuild)
    {
        OnGuildLeft.Broadcast();
    }
}
