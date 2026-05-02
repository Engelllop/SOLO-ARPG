#include "World/N1/ASOLON1_VoiceEventActor.h"
#include "Components/BoxComponent.h"
#include "Character/SOLOPlayerCharacter.h"
#include "GameFramework/SOLOGameInstance.h"
#include "Kismet/GameplayStatics.h"

ASOLON1_VoiceEventActor::ASOLON1_VoiceEventActor()
{
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	RootComponent = TriggerVolume;
	TriggerVolume->SetBoxExtent(FVector(300.f, 300.f, 200.f));
	TriggerVolume->SetCollisionProfileName(TEXT("Trigger"));
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ASOLON1_VoiceEventActor::OnTriggerOverlap);
}

void ASOLON1_VoiceEventActor::BeginPlay()
{
	Super::BeginPlay();

	if (StoryFlagOnComplete.IsNone()) return;

	auto* GI = Cast<USOLOGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI && GI->HasStoryFlag(StoryFlagOnComplete))
		bTriggered = true;
}

void ASOLON1_VoiceEventActor::OnTriggerOverlap(UPrimitiveComponent* Comp, AActor* Other,
	UPrimitiveComponent* OtherComp, int32 Idx, bool bSweep, const FHitResult& Hit)
{
	if (bTriggered) return;
	if (!Cast<ASOLOPlayerCharacter>(Other)) return;

	bTriggered = true;
	CurrentLineIndex = 0;
	PlayNextLine();
}

void ASOLON1_VoiceEventActor::PlayNextLine()
{
	if (CurrentLineIndex >= VoiceLines.Num())
	{
		if (!StoryFlagOnComplete.IsNone())
		{
			auto* GI = Cast<USOLOGameInstance>(UGameplayStatics::GetGameInstance(this));
			if (GI) GI->SetStoryFlag(StoryFlagOnComplete, true);
		}
		BP_OnEventFinished();
		OnEventFinished.Broadcast();
		return;
	}

	const FText& Line = VoiceLines[CurrentLineIndex];
	float Delay = LineDelays.IsValidIndex(CurrentLineIndex) ? LineDelays[CurrentLineIndex] : 3.f;

	BP_OnLineStarted(CurrentLineIndex, Line);
	OnVoiceLine.Broadcast(CurrentLineIndex);

	CurrentLineIndex++;
	GetWorld()->GetTimerManager().SetTimer(LineTimer, this,
		&ASOLON1_VoiceEventActor::PlayNextLine, Delay, false);
}
