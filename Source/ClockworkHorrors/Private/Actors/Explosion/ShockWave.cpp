// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.


#include "Actors/Explosion/ShockWave.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/StatusEffectType.h"
#include "Components/MeshComponent.h"
// Sets default values
AShockWave::AShockWave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;

	CollisionSphere->InitSphereRadius(0.f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	SphereMesh->SetupAttachment(RootComponent);

	SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereMesh->SetCastShadow(false);
	SphereMesh->SetWorldScale3D(FVector::ZeroVector);

	ShockWaveRadius = 500.f;
	ShockWaveForce = 1000.f;
	ShockWaveSpeed = 1000.f;
	ShockWaveDamage = 50.f;

	CurrentRadius = 0.f;

}

// Called when the game starts or when spawned
void AShockWave::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AShockWave::OnSphereBeginOverlap);
}

void AShockWave::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || HitActors.Contains(OtherActor) || OtherActor == GetOwner())
	{
		return;
	}

	HitActors.Add(OtherActor);

	OtherActor->TakeDamage(ShockWaveDamage, FDamageEvent(), GetInstigatorController(), this);

}

// Called every frame
void AShockWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentRadius >= ShockWaveRadius)
	{
		Destroy();
		return;
	}

	CurrentRadius = FMath::Min(CurrentRadius + (ShockWaveSpeed * DeltaTime), ShockWaveRadius);
	CollisionSphere->SetSphereRadius(CurrentRadius);
	if (const UStaticMesh* Mesh = SphereMesh->GetStaticMesh())
	{
		const float MeshBaseRadius = Mesh->GetBounds().SphereRadius;
		if (MeshBaseRadius > KINDA_SMALL_NUMBER)
		{
			SphereMesh->SetWorldScale3D(FVector(CurrentRadius / MeshBaseRadius));
		}
	}

}

UStatusEffectType* AShockWave::GetStatusEffectPayload(AActor* TargetActor) const
{
	UStatusEffectType* StatusEffect = NewObject<UStatusEffectType>();

	StatusEffect->Effect = STATUSEFFECT::KnockBack;
	StatusEffect->TickDamage = ShockWaveForce;
	StatusEffect->Duration = ShockWaveForce / 4;

	if (TargetActor)
	{
		FVector Direction = TargetActor->GetActorLocation() - GetActorLocation();
		Direction.Normalize();
		StatusEffect->KnockbackDirection = Direction;
	}
	else
	{
		StatusEffect->KnockbackDirection = FVector::ZeroVector;
	}

	UStatusEffectType* StunEffect = NewObject<UStatusEffectType>();
	StunEffect->Effect = STATUSEFFECT::Stunned;
	StunEffect->Duration = StunDuration;

	StatusEffect->NextStatusEffect = StunEffect;

	return StatusEffect;
}

