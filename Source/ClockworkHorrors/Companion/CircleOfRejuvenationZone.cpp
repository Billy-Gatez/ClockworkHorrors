#include "CircleOfRejuvenationZone.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Utils/HealthComponent.h"

ACircleOfRejuvenationZone::ACircleOfRejuvenationZone()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(RootComponent);
	OverlapSphere->SetCollisionProfileName(TEXT("Trigger"));
	OverlapSphere->SetSphereRadius(ZoneRadius);

	ZoneNiagaraFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ZoneNiagaraFX"));
	ZoneNiagaraFX->SetupAttachment(RootComponent);
}

void ACircleOfRejuvenationZone::BeginPlay()
{
	Super::BeginPlay();

	// Update sphere collision radius
	if (OverlapSphere)
	{
		OverlapSphere->SetSphereRadius(ZoneRadius);
	}

	// Pass dynamic radius parameter to Niagara System
	if (ZoneNiagaraFX)
	{
		ZoneNiagaraFX->SetFloatParameter(FName("User.ZoneRadius"), ZoneRadius);
	}

	// 1. Initial burst heal
	PerformPulseHeal();

	// 2. Set repeating timer for tick heals
	GetWorldTimerManager().SetTimer(
		HealTimerHandle,
		this,
		&ACircleOfRejuvenationZone::PerformPulseHeal,
		HealInterval,
		true
	);

	// 3. Expiration timer
	GetWorldTimerManager().SetTimer(
		ExpirationTimerHandle,
		this,
		&ACircleOfRejuvenationZone::ExpireZone,
		Duration,
		false
	);
}

void ACircleOfRejuvenationZone::PerformPulseHeal()
{
	if (!OverlapSphere) return;

	TArray<AActor*> OverlappingActors;
	OverlapSphere->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (!IsValid(Actor)) continue;

		if (UHealthComponent* HealthComp = Actor->FindComponentByClass<UHealthComponent>())
		{
			HealthComp->Heal(HealPerTick);
		}
	}
}

void ACircleOfRejuvenationZone::ExpireZone()
{
	GetWorldTimerManager().ClearTimer(HealTimerHandle);
	GetWorldTimerManager().ClearTimer(ExpirationTimerHandle);

	// Deactivate Niagara gracefully so active particles finish living out their lifetime
	if (ZoneNiagaraFX)
	{
		ZoneNiagaraFX->Deactivate();
	}

	// Set short delay or destroy actor
	SetLifeSpan(1.0f); // Gives 1 second for particle tail-off before hard destruction
}

void ACircleOfRejuvenationZone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(HealTimerHandle);
	GetWorldTimerManager().ClearTimer(ExpirationTimerHandle);

	Super::EndPlay(EndPlayReason);
}