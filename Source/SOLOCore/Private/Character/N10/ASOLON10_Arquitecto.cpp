#include "Character/N10/ASOLON10_Arquitecto.h"
#include "World/N10/ASOLON10_PlayerClone.h"
#include "Save/SOLOSaveSubsystem.h"
#include "Save/SOLOSaveGame.h"
#include "Kismet/GameplayStatics.h"

ASOLON10_Arquitecto::ASOLON10_Arquitecto()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASOLON10_Arquitecto::BeginPlay()
{
    Super::BeginPlay();
    SetActorScale3D(FVector(1.8f, 1.8f, 1.8f));
    LocalPhase = 1;
    bHasShownCorpEmails = false;
    bHasShownArchive = false;
    bCentinelaDefeated = false;
    bIsChoiceMade = false;

    // Read story flags from save
    USOLOSaveSubsystem* SaveSystem = GetGameInstance()->GetSubsystem<USOLOSaveSubsystem>();
    if (SaveSystem && SaveSystem->GetCurrentSave())
    {
        USOLOSaveGame* Save = SaveSystem->GetCurrentSave();
        bHasFragment = Save->CollectedFragments.Contains(TEXT("Fragment_Arquitecto"));
        bHasListenedVerdugo = Save->StoryFlags.FindRef(TEXT("N9.VerdugoListened"));
        bAteInN4 = Save->StoryFlags.FindRef(TEXT("N4.Ate"));
        bDidNotNameCriada = !Save->StoryFlags.FindRef(TEXT("N8.CriadaNamed"));
        bListenedVerdugo = Save->StoryFlags.FindRef(TEXT("N9.VerdugoListened"));
        bLookedIntoAbyss = Save->bLookedIntoAbyss;
        bHasAllAbilities = Save->UnlockedSkillIDs.Num() >= VALUE_N10_MIN_SKILLS;
    }

    OnFirstMeeting();
}

void ASOLON10_Arquitecto::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASOLON10_Arquitecto::AdvanceLocalPhase()
{
    LocalPhase++;
    if (LocalPhase == 2 && !bHasShownCorpEmails)
    {
        bHasShownCorpEmails = true;
        ShowCorpEmails();
    }
    else if (LocalPhase == 3 && !bHasShownArchive)
    {
        bHasShownArchive = true;
        ShowArchive();
    }
    else if (LocalPhase == 4 && !bCentinelaDefeated)
    {
        SpawnCentinela();
        OnCentinelaPhase1();
    }
}

void ASOLON10_Arquitecto::SpawnCentinela()
{
    if (!CentinelaClass) return;

    FVector SpawnLoc = GetActorLocation() + FVector(0.0f, 0.0f, 200.0f);
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    AActor* Centinela = GetWorld()->SpawnActor<AActor>(CentinelaClass, SpawnLoc, FRotator::ZeroRotator, Params);
    if (Centinela)
    {
        ActiveCentinelaParts.Add(Centinela);
    }
}

void ASOLON10_Arquitecto::SpawnDecisionClones()
{
    if (!DecisionCloneClass) return;

    // Spawn clones representing the player's key decisions
    TArray<FString> DecisionTags;
    if (bAteInN4) DecisionTags.Add(TEXT("EL_QUE_COMIO"));
    else DecisionTags.Add(TEXT("EL_QUE_RECHAZO"));

    // Siempre Thornwall
    DecisionTags.Add(TEXT("EL_QUE_SE_FUE_DE_THORNWALL"));

    if (bListenedVerdugo) DecisionTags.Add(TEXT("EL_QUE_ESCUCHO"));
    else DecisionTags.Add(TEXT("EL_QUE_ATACO"));

    for (int32 i = 0; i < DecisionTags.Num(); i++)
    {
        FVector SpawnLoc = GetActorLocation() + FVector(FMath::FRandRange(-500.0f, 500.0f),
                                                         FMath::FRandRange(-500.0f, 500.0f), 0.0f);
        FActorSpawnParameters Params;
        Params.Owner = this;
        AActor* Clone = GetWorld()->SpawnActor<AActor>(DecisionCloneClass, SpawnLoc, FRotator::ZeroRotator, Params);
        if (Clone)
        {
            ActiveCentinelaParts.Add(Clone);
        }
    }

    OnCentinelaPhase2();
}

void ASOLON10_Arquitecto::SetCentinelaDefeated()
{
    bCentinelaDefeated = true;
    OnCentinelaPhase3();
    OnSphereAppears();
}

bool ASOLON10_Arquitecto::CheckFinalBConditions() const
{
    // v3: Fragmento del Arquitecto + escuchar al Verdugo + proteger la esfera
    return bHasFragment && bHasListenedVerdugo;
}

bool ASOLON10_Arquitecto::CheckFinalCConditions() const
{
    // v3: comprension tematica — opciones mas oscuras
    return bAteInN4 && bDidNotNameCriada && bListenedVerdugo;
}

void ASOLON10_Arquitecto::ChooseEscape()
{
    bIsChoiceMade = true;
    bChoseEscape = true;
    OnEndingA();
}

void ASOLON10_Arquitecto::ChooseStay()
{
    bIsChoiceMade = true;
    bChoseStay = true;

    if (CheckFinalBConditions())
    {
        OnEndingB();
    }
    else
    {
        OnEndingBRequirementCheck();
        OnEndingA();
    }
}

void ASOLON10_Arquitecto::ChooseDestroy()
{
    bIsChoiceMade = true;
    bChoseDestroy = true;

    if (CheckFinalCConditions())
    {
        OnEndingC();
    }
    else
    {
        // If conditions not met, defaults to A
        OnEndingA();
    }
}

float ASOLON10_Arquitecto::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (LocalPhase >= 5) return 0.0f;
    return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ASOLON10_Arquitecto::OnDefeated()
{
    if (!bIsChoiceMade)
    {
        AdvanceLocalPhase();
        if (LocalPhase >= 5)
        {
            ChooseEscape();
        }
    }
}
