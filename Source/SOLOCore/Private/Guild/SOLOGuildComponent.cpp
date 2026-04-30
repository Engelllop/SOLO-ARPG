#include "Guild/SOLOGuildComponent.h"
#include "Guild/SOLOGuildManager.h"
#include "GameFramework/SOLOGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

// â”€â”€â”€ CONSTRUCTOR â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

USOLOGuildComponent::USOLOGuildComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

// â”€â”€â”€ LIFECYCLE â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void USOLOGuildComponent::BeginPlay()
{
    Super::BeginPlay();

    // Inicializar LocalPlayerID desde la GameInstance (EOS Product User ID)
    USOLOGameInstance* GI = GetWorld()
        ? GetWorld()->GetGameInstance<USOLOGameInstance>()
        : nullptr;

    if (GI)
    {
        LocalPlayerID = GI->GetLocalUserId();
    }
}

void USOLOGuildComponent::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(USOLOGuildComponent, CurrentGuild);
    DOREPLIFETIME(USOLOGuildComponent, bIsInGuild);
}

// â”€â”€â”€ CONSULTAS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

bool USOLOGuildComponent::IsInGuild() const
{
    return bIsInGuild && CurrentGuild.IsValid();
}

FString USOLOGuildComponent::GetGuildName() const
{
    return IsInGuild() ? CurrentGuild.GuildName : FString();
}

FString USOLOGuildComponent::GetGuildTag() const
{
    return IsInGuild() ? CurrentGuild.GuildTag : FString();
}

EGuildRank USOLOGuildComponent::GetLocalPlayerRank() const
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

bool USOLOGuildComponent::HasPermission(EGuildPermission Permission) const
{
    return HasGuildPermission(GetLocalPlayerRank(), Permission);
}

FGuildMember* USOLOGuildComponent::FindMember(const FString& PlayerID)
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

TArray<FGuildMember> USOLOGuildComponent::GetOnlineMembers() const
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

// â”€â”€â”€ ACTUALIZACION DE DATOS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void USOLOGuildComponent::UpdateGuildData(const FGuildData& NewData)
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

void USOLOGuildComponent::LeaveGuild()
{
    if (!IsInGuild())
    {
        return;
    }

    // Notificar al GuildManager antes de limpiar los datos locales
    USOLOGuildManager* GuildManager = GetWorld()
        ? GetWorld()->GetGameInstance()->GetSubsystem<USOLOGuildManager>()
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

// â”€â”€â”€ OnRep CALLBACKS â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€

void USOLOGuildComponent::OnRep_CurrentGuild()
{
    OnGuildDataUpdated.Broadcast(CurrentGuild);
}

void USOLOGuildComponent::OnRep_bIsInGuild()
{
    if (!bIsInGuild)
    {
        OnGuildLeft.Broadcast();
    }
}
