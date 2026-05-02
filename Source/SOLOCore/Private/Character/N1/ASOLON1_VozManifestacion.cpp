#include "Character/N1/ASOLON1_VozManifestacion.h"
#include "Character/N1/ASOLON1_VozReflejo.h"
#include "AbilitySystemComponent.h"
#include "Data/N1/SOLON1_Constants.h"

ASOLON1_VozManifestacion::ASOLON1_VozManifestacion()
{
	bIsBoss = false;
	EnemyType = EEnemyType::Boss;
	EnemyLevel = 5;
	XPReward = 0.f; // XP is on the orchestrating LaVozBoss
	BossDisplayName = FText::FromString(TEXT("Manifestación de la Voz"));
	Tags.Add(FName(N1::EnemyTag::VozManifestacion));
}

void ASOLON1_VozManifestacion::BeginPlay()
{
	Super::BeginPlay();
	ApplyInvulnerability();
}

void ASOLON1_VozManifestacion::LinkReflejo(ASOLON1_VozReflejo* InReflejo)
{
	ReflejoRef = InReflejo;
	if (InReflejo)
	{
		InReflejo->OnReflejoHit.AddDynamic(this, &ASOLON1_VozManifestacion::OnReflejoHit);
		InReflejo->OnReflejoDestroyed.AddDynamic(this, &ASOLON1_VozManifestacion::OnReflejoDestroyed);
	}
}

void ASOLON1_VozManifestacion::ApplyInvulnerability()
{
	if (!InvulnerableGE) return;
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC) return;

	FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(InvulnerableGE, 1, Ctx);
	if (Spec.IsValid())
		InvulnerableHandle = ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

void ASOLON1_VozManifestacion::RemoveInvulnerability()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (ASC && InvulnerableHandle.IsValid())
		ASC->RemoveActiveGameplayEffect(InvulnerableHandle);
}

void ASOLON1_VozManifestacion::BeginVulnerableWindow()
{
	if (bVulnerable) return;
	bVulnerable = true;
	RemoveInvulnerability();
	BP_OnVulnerableStart();

	GetWorld()->GetTimerManager().SetTimer(VulnerableTimer, this,
		&ASOLON1_VozManifestacion::EndVulnerableWindow, VulnerableWindowDuration, false);
}

void ASOLON1_VozManifestacion::EndVulnerableWindow()
{
	bVulnerable = false;
	ApplyInvulnerability();
	BP_OnVulnerableEnd();
}

void ASOLON1_VozManifestacion::OnReflejoHit()
{
	BeginVulnerableWindow();
}

void ASOLON1_VozManifestacion::OnReflejoDestroyed()
{
	RemoveInvulnerability();
	bVulnerable = true;
	GetWorld()->GetTimerManager().ClearTimer(VulnerableTimer);

	// Final kill — force death on the manifestation
	Die_Implementation(nullptr);
}

void ASOLON1_VozManifestacion::Die_Implementation(AActor* InstigatorActor)
{
	GetWorld()->GetTimerManager().ClearTimer(VulnerableTimer);
	BP_OnDefeated();
	OnDefeated.Broadcast();
	Super::Die_Implementation(InstigatorActor);
}
