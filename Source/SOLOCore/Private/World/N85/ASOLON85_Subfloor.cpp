#include "World/N85/ASOLON85_Subfloor.h"
#include "Save/SOLOSaveSubsystem.h"
#include "Save/SOLOSaveGame.h"

ASOLON85_Subfloor::ASOLON85_Subfloor()
{
    PrimaryActorTick.bCanEverTick = false;
    DeskNoteText = FText::FromString(
        "Permiso medico — 3 dias.\n"
        "Motivo: llevo 9 anos viendo a los NPCs preguntarse si son reales.\n"
        "Deberian ser solo datos. Pero empece a llamarlos por su nombre.\n"
        "No voy a volver.\n"
        "— Arquitecto.\n"
        "Iteracion 93."
    );
}

void ASOLON85_Subfloor::BeginPlay()
{
    Super::BeginPlay();

    USOLOSaveSubsystem* SaveSystem = GetGameInstance()->GetSubsystem<USOLOSaveSubsystem>();
    if (SaveSystem && SaveSystem->GetCurrentSave())
    {
        bHasBeenEntered = SaveSystem->GetCurrentSave()->bSawN85Subfloor;
        bHasReadNote = SaveSystem->GetCurrentSave()->StoryFlags.FindRef(TEXT("N8.5.PermisoLeido"));
    }
}

void ASOLON85_Subfloor::ReadDeskNote()
{
    bHasReadNote = true;

    USOLOSaveSubsystem* SaveSystem = GetGameInstance()->GetSubsystem<USOLOSaveSubsystem>();
    if (SaveSystem && SaveSystem->GetCurrentSave())
    {
        SaveSystem->GetCurrentSave()->StoryFlags.Add(TEXT("N8.5.PermisoLeido"), true);
    }

    OnReadDeskNote();
}
