#include "World/N1/ASOLON1_EchoShadow.h"
#include "Engine/OverlapResult.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Data/N1/SOLON1_Constants.h"

ASOLON1_EchoShadow::ASOLON1_EchoShadow()
{
	bIsBoss = false;
	EnemyType = EEnemyType::Common;
	EnemyLevel = 1;
	XPReward = 0.f;
	Tags.Add(FName(N1::EnemyTag::EchoShadow));
}

void ASOLON1_EchoShadow::BeginPlay()
{
	Super::BeginPlay();
	if (USkeletalMeshComponent* M = GetMesh())
	{
		M->SetRenderCustomDepth(true);
		M->SetCustomDepthStencilValue(1);
	}
	BP_OnEchoSpawned();
}

void ASOLON1_EchoShadow::InitEcho(AActor* SourceActor, float AttackDelay)
{
	EchoSource = SourceActor;
	float Delay = (AttackDelay > 0.f) ? AttackDelay : DefaultAttackDelay;
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, this,
		&ASOLON1_EchoShadow::ExecuteAttack, Delay, false);
}

void ASOLON1_EchoShadow::ExecuteAttack()
{
	BP_OnEchoAttack();

	if (!EchoDamageGE) { SetLifeSpan(LifespanAfterAttack); return; }

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponent();
	if (!SourceASC) { SetLifeSpan(LifespanAfterAttack); return; }

	FVector Origin = GetActorLocation();
	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, ECC_Pawn,
		FCollisionShape::MakeSphere(AttackRadius));

	FGameplayEffectContextHandle Ctx = SourceASC->MakeEffectContext();
	Ctx.AddInstigator(this, this);
	FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(EchoDamageGE, 1, Ctx);

	for (const FOverlapResult& O : Overlaps)
	{
		if (O.GetActor() == this) continue;
		if (auto* IASI = Cast<IAbilitySystemInterface>(O.GetActor()))
		{
			if (auto* TargetASC = IASI->GetAbilitySystemComponent())
			{
				if (Spec.IsValid())
					SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
			}
		}
	}

	SetLifeSpan(LifespanAfterAttack);
}
