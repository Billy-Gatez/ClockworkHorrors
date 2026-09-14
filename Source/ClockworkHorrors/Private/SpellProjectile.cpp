// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#include "SpellProjectile.h"

#include "Spells.h"
#include "SpellLingeringField.h"
#include "BaseCharacter.h"
#include "Components/ManaComponent.h"
#include "Enemy.h"

#include "Components/SceneComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

ASpellProjectile::ASpellProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	SetRootComponent(CollisionSphere);

	CollisionSphere->InitSphereRadius(20.0f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionSphere->SetNotifyRigidBodyCollision(true);
	CollisionSphere->SetGenerateOverlapEvents(true);

	ProjectileAppearance = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileAppearance"));
	ProjectileAppearance->SetupAttachment(CollisionSphere);
	ProjectileAppearance->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionSphere;
	ProjectileMovement->InitialSpeed = 1500.0f;
	ProjectileMovement->MaxSpeed = 1500.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->Bounciness = 1.0f;
	ProjectileMovement->Friction = 0.0f;
	ProjectileMovement->BounceVelocityStopSimulatingThreshold = 0.0f;
	ProjectileMovement->bIsHomingProjectile = false;
	ProjectileMovement->HomingAccelerationMagnitude = 5000.0f;

	CastingActor = nullptr;
	SpellDefinition = nullptr;
	CastingCollisionComponent = nullptr;
	CasterManaComponent = nullptr;

	bShowProjectileHitbox = false;
	bShowLingeringHitbox = false;

	CurrentBounceCount = 0;
	CurrentPierceCount = 0;

	CurrentImpactDamage = 0.0f;
	ChargePercent = 0.0f;

	ScaleChangeElapsedSeconds = 0.0f;
	BaseProjectileCollisionRadius = 20.0f;
	CurrentScaleDamageMultiplier = 1.0f;
	BaseProjectileAppearanceScale = FVector::OneVector;

	bPausedForRedirect = false;
	bManaRecoveryBlockApplied = false;
	RedirectProjectileSpeed = 0.0f;
	PreRedirectVelocity = FVector::ZeroVector;

	RiderCharacter = nullptr;
	bRiderAttached = false;
	bSavedImpartBaseVelocityX = true;
	bSavedImpartBaseVelocityY = true;
	bSavedImpartBaseVelocityZ = true;
	bSavedImpartBaseAngularVelocity = true;
	bSavedIgnoreBaseRotation = false;
	ProjectileAppearanceBaseRelativeLocation = FVector::ZeroVector;
	ProjectileAppearanceBaseRelativeRotation = FRotator::ZeroRotator;

	bGravityActive = false;
	bGravityProjectileAnchored = false;
	GravityActivationDelayRemaining = 0.0f;
}

void ASpellProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->OnComponentHit.AddDynamic(this, &ASpellProjectile::ProjectileImpact);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASpellProjectile::ProjectileOverlap);

	if (ProjectileAppearance)
	{
		ProjectileAppearanceBaseRelativeLocation = ProjectileAppearance->GetRelativeLocation();
		ProjectileAppearanceBaseRelativeRotation = ProjectileAppearance->GetRelativeRotation();
		BaseProjectileAppearanceScale = ProjectileAppearance->GetRelativeScale3D();
	}
}

void ASpellProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ReleaseGravityAffectedEnemies();
	DetachProjectileRider();

	if (IsValid(CastingCollisionComponent))
	{
		CastingCollisionComponent->IgnoreActorWhenMoving(this, false);
	}

	if (bManaRecoveryBlockApplied && IsValid(CasterManaComponent))
	{
		CasterManaComponent->RemoveManaRecoveryBlock();
		bManaRecoveryBlockApplied = false;
	}

	Super::EndPlay(EndPlayReason);
}

void ASpellProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateProjectileScaleOverTime(DeltaTime);

	if (SpellDefinition && SpellDefinition->bLastsWhileCasterHasMana)
	{
		UManaComponent* ManaComponent = IsValid(CastingActor)
			? CastingActor->FindComponentByClass<UManaComponent>()
			: nullptr;

		if (!IsValid(ManaComponent) || ManaComponent->GetCurrentMana() <= 0.0f)
		{
			Destroy();
			return;
		}
	}

	if (SpellDefinition && SpellDefinition->bRedirectableProjectile && !bGravityActive)
	{
		if (bPausedForRedirect && SpellDefinition->bShowRedirectAimLine)
		{
			FVector AimPoint = FVector::ZeroVector;

			if (GetRedirectAimPoint(AimPoint))
			{
				DrawDebugLine(
					GetWorld(),
					GetActorLocation(),
					AimPoint,
					FColor::Cyan,
					false,
					0.0f,
					0,
					FMath::Max(SpellDefinition->RedirectAimLineThickness, 0.1f)
				);
			}
		}

		const float ManaDrain = FMath::Max(SpellDefinition->RedirectManaDrainPerSecond, 0.0f) * DeltaTime;

		if (ManaDrain > 0.0f)
		{
			UManaComponent* ManaComponent = IsValid(CastingActor)
				? CastingActor->FindComponentByClass<UManaComponent>()
				: nullptr;

			if (!IsValid(ManaComponent))
			{
				Destroy();
				return;
			}

			if (!ManaComponent->ConsumeMana(ManaDrain))
			{
				ManaComponent->SetCurrentMana(0.0f);
				Destroy();
				return;
			}
		}
	}

	if (
		SpellDefinition &&
		SpellDefinition->bCreatesGravity &&
		!bGravityActive &&
		SpellDefinition->GravityActivationMode == EGravityActivationMode::AfterTime
		)
	{
		GravityActivationDelayRemaining -= DeltaTime;

		if (GravityActivationDelayRemaining <= 0.0f)
		{
			ActivateGravity();
		}
	}

	if (bGravityActive)
	{
		UpdateGravity(DeltaTime);
	}

	UpdateProjectileRider();

	if (SpellDefinition && SpellDefinition->bAllowProjectileRiding && ProjectileMovement && ProjectileAppearance && CollisionSphere)
	{
		if (!ProjectileMovement->Velocity.IsNearlyZero())
		{
			const FQuat TravelRotation = ProjectileMovement->Velocity.ToOrientationQuat();
			const FVector VisualLocation =
				CollisionSphere->GetComponentLocation() +
				TravelRotation.RotateVector(ProjectileAppearanceBaseRelativeLocation);

			const FQuat VisualRotation =
				TravelRotation * ProjectileAppearanceBaseRelativeRotation.Quaternion();

			ProjectileAppearance->SetWorldLocationAndRotation(
				VisualLocation,
				VisualRotation
			);
		}
	}

	if (
		!bPausedForRedirect &&
		!bGravityActive &&
		!bGravityProjectileAnchored &&
		SpellDefinition &&
		SpellDefinition->bProjectileHoming
		)
	{
		USceneComponent* HomingTargetComponent = ProjectileMovement->HomingTargetComponent.Get();

		if (!IsValid(HomingTargetComponent) || DamagedActors.Contains(HomingTargetComponent->GetOwner()))
		{
			AcquireHomingTarget();
		}
	}

	if (!bShowProjectileHitbox || !CollisionSphere)
	{
		return;
	}

	DrawDebugSphere(
		GetWorld(),
		CollisionSphere->GetComponentLocation(),
		CollisionSphere->GetScaledSphereRadius(),
		24,
		FColor::Red,
		false,
		0.0f,
		0,
		1.5f
	);
}


void ASpellProjectile::UpdateProjectileRider()
{
	if (!SpellDefinition || !SpellDefinition->bAllowProjectileRiding)
	{
		DetachProjectileRider();
		return;
	}

	ACharacter* Character = Cast<ACharacter>(CastingActor);

	if (!IsValid(Character))
	{
		DetachProjectileRider();
		return;
	}

	UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();

	if (!IsValid(CharacterMovement))
	{
		DetachProjectileRider();
		return;
	}

	if (bRiderAttached)
	{
		if (RiderCharacter != Character || CharacterMovement->IsFalling())
		{
			DetachProjectileRider();
			return;
		}

		Character->SetBase(CollisionSphere);
		CharacterMovement->Velocity.X = 0.0f;
		CharacterMovement->Velocity.Y = 0.0f;

		return;
	}

	const bool bStandingOnProjectile =
		Character->GetMovementBase() == CollisionSphere;

	if (!CharacterMovement->IsFalling() && bStandingOnProjectile)
	{
		AttachProjectileRider(Character);
	}
}

void ASpellProjectile::AttachProjectileRider(ACharacter* Character)
{
	if (!IsValid(Character) || bRiderAttached)
	{
		return;
	}

	UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();

	if (!IsValid(CharacterMovement))
	{
		return;
	}

	RiderCharacter = Character;
	bRiderAttached = true;

	bSavedImpartBaseVelocityX = CharacterMovement->bImpartBaseVelocityX;
	bSavedImpartBaseVelocityY = CharacterMovement->bImpartBaseVelocityY;
	bSavedImpartBaseVelocityZ = CharacterMovement->bImpartBaseVelocityZ;
	bSavedImpartBaseAngularVelocity = CharacterMovement->bImpartBaseAngularVelocity;
	bSavedIgnoreBaseRotation = CharacterMovement->bIgnoreBaseRotation;

	CharacterMovement->bImpartBaseVelocityX = false;
	CharacterMovement->bImpartBaseVelocityY = false;
	CharacterMovement->bImpartBaseVelocityZ = false;
	CharacterMovement->bImpartBaseAngularVelocity = false;
	CharacterMovement->bIgnoreBaseRotation = true;

	CharacterMovement->Velocity.X = 0.0f;
	CharacterMovement->Velocity.Y = 0.0f;

	Character->SetBase(CollisionSphere);
}

void ASpellProjectile::DetachProjectileRider()
{
	if (!bRiderAttached)
	{
		return;
	}

	if (IsValid(RiderCharacter))
	{
		UCharacterMovementComponent* CharacterMovement = RiderCharacter->GetCharacterMovement();

		if (IsValid(CharacterMovement))
		{
			CharacterMovement->bImpartBaseVelocityX = bSavedImpartBaseVelocityX;
			CharacterMovement->bImpartBaseVelocityY = bSavedImpartBaseVelocityY;
			CharacterMovement->bImpartBaseVelocityZ = bSavedImpartBaseVelocityZ;
			CharacterMovement->bImpartBaseAngularVelocity = bSavedImpartBaseAngularVelocity;
			CharacterMovement->bIgnoreBaseRotation = bSavedIgnoreBaseRotation;
		}

		if (RiderCharacter->GetMovementBase() == CollisionSphere)
		{
			RiderCharacter->SetBase(nullptr);
		}
	}

	RiderCharacter = nullptr;
	bRiderAttached = false;
}

void ASpellProjectile::InitializeProjectile(AActor* InCastingActor, ASpells* InSpellDefinition, const FVector& InTravelDirection)
{
	InitializeProjectile(InCastingActor, InSpellDefinition, InTravelDirection, 0.0f);
}

void ASpellProjectile::InitializeProjectile(AActor* InCastingActor, ASpells* InSpellDefinition, const FVector& InTravelDirection, float InChargePercent)
{
	CastingActor = InCastingActor;
	SpellDefinition = InSpellDefinition;

	if (!SpellDefinition)
	{
		Destroy();
		return;
	}

	ChargePercent = FMath::Clamp(InChargePercent, 0.0f, 1.0f);

	ScaleChangeElapsedSeconds = 0.0f;
	CurrentScaleDamageMultiplier = 1.0f;

	CurrentBounceCount = 0;
	CurrentPierceCount = 0;
	DamagedActors.Empty();

	ReleaseGravityAffectedEnemies();
	bGravityActive = false;
	bGravityProjectileAnchored = false;
	GravityActivationDelayRemaining =
		SpellDefinition->bCreatesGravity &&
		SpellDefinition->GravityActivationMode == EGravityActivationMode::AfterTime
		? FMath::Max(SpellDefinition->GravityActivationDelaySeconds, 0.0f)
		: 0.0f;

	float ProjectileSpeed = SpellDefinition->ProjectileTravelSpeed;
	float ProjectileRadius = SpellDefinition->ProjectileCollisionRadius;

	CurrentImpactDamage = SpellDefinition->ImpactDamageStrength;

	if (SpellDefinition->bChargeableSpell)
	{
		if (SpellDefinition->bChargeAffectsImpactDamage)
		{
			const float DamageMultiplier = FMath::Lerp(
				SpellDefinition->MinimumChargeDamageMultiplier,
				SpellDefinition->MaximumChargeDamageMultiplier,
				ChargePercent
			);

			CurrentImpactDamage *= DamageMultiplier;
		}

		if (SpellDefinition->bChargeAffectsProjectileSize)
		{
			const float SizeMultiplier = FMath::Lerp(
				SpellDefinition->MinimumChargeSizeMultiplier,
				SpellDefinition->MaximumChargeSizeMultiplier,
				ChargePercent
			);

			ProjectileRadius *= SizeMultiplier;

			ProjectileAppearance->SetRelativeScale3D(
				ProjectileAppearance->GetRelativeScale3D() *
				SizeMultiplier
			);
		}

		if (SpellDefinition->bChargeAffectsProjectileSpeed)
		{
			const float SpeedMultiplier = FMath::Lerp(
				SpellDefinition->MinimumChargeSpeedMultiplier,
				SpellDefinition->MaximumChargeSpeedMultiplier,
				ChargePercent
			);

			ProjectileSpeed *= SpeedMultiplier;
		}
	}

	BaseProjectileCollisionRadius = ProjectileRadius;
	BaseProjectileAppearanceScale = ProjectileAppearance
		? ProjectileAppearance->GetRelativeScale3D()
		: FVector::OneVector;

	CollisionSphere->SetSphereRadius(ProjectileRadius);

	if (SpellDefinition->bProjectileScaleChangesOverTime)
	{
		const float StartingScale = FMath::Max(
			SpellDefinition->ProjectileStartingScale,
			0.01f
		);

		if (ProjectileAppearance)
		{
			ProjectileAppearance->SetRelativeScale3D(
				BaseProjectileAppearanceScale *
				StartingScale
			);
		}

		if (SpellDefinition->bProjectileScaleAffectsCollision)
		{
			CollisionSphere->SetSphereRadius(
				BaseProjectileCollisionRadius *
				StartingScale
			);
		}

		if (SpellDefinition->bProjectileScaleAffectsDamage)
		{
			CurrentScaleDamageMultiplier = FMath::Max(
				SpellDefinition->ProjectileStartingDamageMultiplier,
				0.0f
			);
		}
	}

	ProjectileMovement->InitialSpeed = ProjectileSpeed;

	ProjectileMovement->MaxSpeed = ProjectileSpeed;

	ProjectileMovement->ProjectileGravityScale = SpellDefinition->ProjectileGravityStrength;

	ProjectileMovement->bRotationFollowsVelocity = !SpellDefinition->bAllowProjectileRiding;

	ProjectileMovement->bShouldBounce =
		SpellDefinition->bProjectileBounces &&
		SpellDefinition->MaximumProjectileBounces > 0;

	ProjectileMovement->Bounciness =
		SpellDefinition->ProjectileBounciness;

	ProjectileMovement->Friction = 0.0f;

	ProjectileMovement->BounceVelocityStopSimulatingThreshold = 0.0f;

	if (SpellDefinition->bProjectilePierces && !SpellDefinition->bProjectileBounces)
	{
		CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
	else
	{
		CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}

	ProjectileMovement->bIsHomingProjectile = false;
	ProjectileMovement->HomingTargetComponent.Reset();

	if (SpellDefinition->bProjectileHoming)
	{
		ProjectileMovement->HomingAccelerationMagnitude = SpellDefinition->ProjectileHomingStrength;

		AcquireHomingTarget();
	}

	FVector TravelDirection = InTravelDirection;

	if (TravelDirection.IsNearlyZero())
	{
		TravelDirection = GetActorForwardVector();
	}

	TravelDirection.Normalize();

	ProjectileMovement->Velocity =
		TravelDirection *
		ProjectileSpeed;

	RedirectProjectileSpeed = ProjectileSpeed;
	PreRedirectVelocity = ProjectileMovement->Velocity;
	bPausedForRedirect = false;

	if (SpellDefinition->bLastsWhileCasterHasMana)
	{
		SetLifeSpan(0.0f);
	}
	else
	{
		SetLifeSpan(SpellDefinition->ProjectileExistenceSeconds);
	}

	if (
		SpellDefinition->bRedirectableProjectile &&
		SpellDefinition->bBlocksManaRecoveryWhileActive &&
		IsValid(CastingActor)
		)
	{
		CasterManaComponent = CastingActor->FindComponentByClass<UManaComponent>();

		if (IsValid(CasterManaComponent))
		{
			CasterManaComponent->AddManaRecoveryBlock();
			bManaRecoveryBlockApplied = true;
		}
	}

	if (CastingActor)
	{
		CollisionSphere->IgnoreActorWhenMoving(CastingActor, true);

		if (!SpellDefinition->bAllowProjectileRiding)
		{
			CastingCollisionComponent = Cast<UPrimitiveComponent>(CastingActor->GetRootComponent());

			if (IsValid(CastingCollisionComponent))
			{
				CastingCollisionComponent->IgnoreActorWhenMoving(this, true);
			}
		}
	}
}

bool ASpellProjectile::PauseForRedirect()
{
	if (!SpellDefinition)
	{
		return false;
	}

	if (!SpellDefinition->bRedirectableProjectile)
	{
		return false;
	}

	if (bPausedForRedirect)
	{
		return false;
	}

	PreRedirectVelocity = ProjectileMovement->Velocity;

	if (PreRedirectVelocity.IsNearlyZero())
	{
		PreRedirectVelocity = GetActorForwardVector() * RedirectProjectileSpeed;
	}

	ProjectileMovement->bIsHomingProjectile = false;
	ProjectileMovement->HomingTargetComponent.Reset();
	ProjectileMovement->StopMovementImmediately();
	ProjectileMovement->Deactivate();

	bPausedForRedirect = true;

	return true;
}

bool ASpellProjectile::RedirectProjectile(const FVector& NewTravelDirection)
{
	if (!SpellDefinition)
	{
		return false;
	}

	if (!SpellDefinition->bRedirectableProjectile)
	{
		return false;
	}

	if (!bPausedForRedirect)
	{
		return false;
	}

	FVector TravelDirection = NewTravelDirection;

	if (TravelDirection.IsNearlyZero())
	{
		TravelDirection = GetActorForwardVector();
	}

	TravelDirection.Normalize();

	ProjectileMovement->Activate(true);
	ProjectileMovement->Velocity = TravelDirection * RedirectProjectileSpeed;
	ProjectileMovement->UpdateComponentVelocity();

	bPausedForRedirect = false;
	PreRedirectVelocity = ProjectileMovement->Velocity;

	if (SpellDefinition->bProjectileHoming)
	{
		ProjectileMovement->HomingAccelerationMagnitude = SpellDefinition->ProjectileHomingStrength;
		AcquireHomingTarget();
	}

	return true;
}

bool ASpellProjectile::IsPausedForRedirect() const
{
	return bPausedForRedirect;
}

void ASpellProjectile::UpdateProjectileScaleOverTime(float DeltaTime)
{
	if (!SpellDefinition || !SpellDefinition->bProjectileScaleChangesOverTime)
	{
		return;
	}

	const float Duration = FMath::Max(
		SpellDefinition->ProjectileScaleChangeDuration,
		0.01f
	);

	ScaleChangeElapsedSeconds = FMath::Min(
		ScaleChangeElapsedSeconds + FMath::Max(DeltaTime, 0.0f),
		Duration
	);

	const float ScaleAlpha = FMath::Clamp(
		ScaleChangeElapsedSeconds / Duration,
		0.0f,
		1.0f
	);

	const float StartingScale = FMath::Max(
		SpellDefinition->ProjectileStartingScale,
		0.01f
	);

	const float EndingScale = FMath::Max(
		SpellDefinition->ProjectileEndingScale,
		0.01f
	);

	const float CurrentScale = FMath::Lerp(
		StartingScale,
		EndingScale,
		ScaleAlpha
	);

	if (ProjectileAppearance)
	{
		ProjectileAppearance->SetRelativeScale3D(
			BaseProjectileAppearanceScale *
			CurrentScale
		);
	}

	if (SpellDefinition->bProjectileScaleAffectsCollision && CollisionSphere)
	{
		CollisionSphere->SetSphereRadius(
			BaseProjectileCollisionRadius *
			CurrentScale
		);
	}

	if (SpellDefinition->bProjectileScaleAffectsDamage)
	{
		CurrentScaleDamageMultiplier = FMath::Lerp(
			FMath::Max(SpellDefinition->ProjectileStartingDamageMultiplier, 0.0f),
			FMath::Max(SpellDefinition->ProjectileEndingDamageMultiplier, 0.0f),
			ScaleAlpha
		);
	}
	else
	{
		CurrentScaleDamageMultiplier = 1.0f;
	}
}

bool ASpellProjectile::ActivateGravity()
{
	if (!SpellDefinition)
	{
		return false;
	}

	if (!SpellDefinition->bCreatesGravity)
	{
		return false;
	}

	if (bGravityActive)
	{
		return false;
	}

	StopForGravityAnchor();
	DetachProjectileRider();

	bGravityActive = true;
	GravityActivationDelayRemaining = 0.0f;

	if (CollisionSphere)
	{
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	SetLifeSpan(FMath::Max(SpellDefinition->GravityLifetime, 0.1f));

	return true;
}

bool ASpellProjectile::IsGravityActive() const
{
	return bGravityActive;
}

void ASpellProjectile::UpdateGravity(float DeltaTime)
{
	if (!bGravityActive || !SpellDefinition || !GetWorld())
	{
		return;
	}

	const float GravityRadius = FMath::Max(SpellDefinition->GravityRadius, 1.0f);
	const float LockRadius = FMath::Clamp(
		SpellDefinition->GravityCenterLockRadius,
		1.0f,
		GravityRadius
	);

	if (SpellDefinition->bShowGravityRadius)
	{
		DrawDebugSphere(
			GetWorld(),
			GetActorLocation(),
			GravityRadius,
			32,
			FColor::Purple,
			false,
			0.0f,
			0,
			2.0f
		);
	}

	TArray<FOverlapResult> OverlapResults;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (IsValid(CastingActor))
	{
		QueryParams.AddIgnoredActor(CastingActor);
	}

	const bool bFoundActors = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ObjectQueryParams,
		FCollisionShape::MakeSphere(GravityRadius),
		QueryParams
	);

	if (!bFoundActors)
	{
		return;
	}

	for (const FOverlapResult& OverlapResult : OverlapResults)
	{
		AEnemy* EnemyCharacter = Cast<AEnemy>(OverlapResult.GetActor());

		if (!IsValid(EnemyCharacter))
		{
			continue;
		}

		UCharacterMovementComponent* EnemyMovement = EnemyCharacter->GetCharacterMovement();

		if (!IsValid(EnemyMovement))
		{
			continue;
		}

		if (SpellDefinition->bGravityDisablesEnemyActions)
		{
			DisableEnemyActionsForGravity(EnemyCharacter);
		}

		const FVector ToGravityCenter =
			GetActorLocation() -
			EnemyCharacter->GetActorLocation();

		const float DistanceToCenter = ToGravityCenter.Size();

		if (DistanceToCenter <= KINDA_SMALL_NUMBER)
		{
			if (SpellDefinition->bGravityLocksAtCenter)
			{
				LockEnemyInGravity(EnemyCharacter);
			}

			continue;
		}

		if (
			SpellDefinition->bGravityLocksAtCenter &&
			DistanceToCenter <= LockRadius
			)
		{
			LockEnemyInGravity(EnemyCharacter);
			continue;
		}

		const TWeakObjectPtr<AEnemy> EnemyKey(EnemyCharacter);

		if (GravityLockedEnemyMovementModes.Contains(EnemyKey))
		{
			continue;
		}

		const FVector PullDirection = ToGravityCenter / DistanceToCenter;
		const float DistancePercent = FMath::Clamp(
			DistanceToCenter / GravityRadius,
			0.0f,
			1.0f
		);

		const float PullStrengthMultiplier = FMath::Lerp(
			1.0f,
			0.35f,
			DistancePercent
		);

		const float PullAcceleration =
			FMath::Max(SpellDefinition->GravityPullStrength, 0.0f) *
			PullStrengthMultiplier;

		EnemyMovement->Velocity +=
			PullDirection *
			PullAcceleration *
			DeltaTime;

		const float MaximumPullSpeed = FMath::Max(
			SpellDefinition->GravityPullStrength,
			600.0f
		);

		EnemyMovement->Velocity =
			EnemyMovement->Velocity.GetClampedToMaxSize(MaximumPullSpeed);
	}
}

void ASpellProjectile::StopForGravityAnchor()
{
	if (bGravityProjectileAnchored)
	{
		return;
	}

	bGravityProjectileAnchored = true;
	bPausedForRedirect = false;

	if (!ProjectileMovement)
	{
		return;
	}

	ProjectileMovement->bIsHomingProjectile = false;
	ProjectileMovement->HomingTargetComponent.Reset();
	ProjectileMovement->StopMovementImmediately();
	ProjectileMovement->Deactivate();
}

void ASpellProjectile::DisableEnemyActionsForGravity(AEnemy* EnemyCharacter)
{
	if (!IsValid(EnemyCharacter))
	{
		return;
	}

	const TWeakObjectPtr<AEnemy> EnemyKey(EnemyCharacter);

	if (!GravityAffectedEnemyTickStates.Contains(EnemyKey))
	{
		GravityAffectedEnemyTickStates.Add(
			EnemyKey,
			EnemyCharacter->IsActorTickEnabled()
		);
	}

	EnemyCharacter->SetActorTickEnabled(false);
}

void ASpellProjectile::LockEnemyInGravity(AEnemy* EnemyCharacter)
{
	if (!IsValid(EnemyCharacter))
	{
		return;
	}

	UCharacterMovementComponent* EnemyMovement = EnemyCharacter->GetCharacterMovement();

	if (!IsValid(EnemyMovement))
	{
		return;
	}

	const TWeakObjectPtr<AEnemy> EnemyKey(EnemyCharacter);

	if (!GravityLockedEnemyMovementModes.Contains(EnemyKey))
	{
		GravityLockedEnemyMovementModes.Add(
			EnemyKey,
			static_cast<uint8>(EnemyMovement->MovementMode)
		);
	}

	if (SpellDefinition && SpellDefinition->bGravityDisablesEnemyActions)
	{
		DisableEnemyActionsForGravity(EnemyCharacter);
	}

	EnemyMovement->StopMovementImmediately();
	EnemyMovement->DisableMovement();
}

void ASpellProjectile::ReleaseGravityAffectedEnemies()
{
	for (const TPair<TWeakObjectPtr<AEnemy>, uint8>& LockedEnemyPair : GravityLockedEnemyMovementModes)
	{
		AEnemy* EnemyCharacter = LockedEnemyPair.Key.Get();

		if (!IsValid(EnemyCharacter))
		{
			continue;
		}

		UCharacterMovementComponent* EnemyMovement = EnemyCharacter->GetCharacterMovement();

		if (!IsValid(EnemyMovement))
		{
			continue;
		}

		EnemyMovement->SetMovementMode(
			static_cast<EMovementMode>(LockedEnemyPair.Value)
		);
	}

	for (const TPair<TWeakObjectPtr<AEnemy>, bool>& AffectedEnemyPair : GravityAffectedEnemyTickStates)
	{
		AEnemy* EnemyCharacter = AffectedEnemyPair.Key.Get();

		if (!IsValid(EnemyCharacter))
		{
			continue;
		}

		EnemyCharacter->SetActorTickEnabled(AffectedEnemyPair.Value);
	}

	GravityLockedEnemyMovementModes.Empty();
	GravityAffectedEnemyTickStates.Empty();
}

bool ASpellProjectile::GetRedirectAimPoint(FVector& OutAimPoint) const
{
	if (!IsValid(SpellDefinition))
	{
		return false;
	}

	if (!SpellDefinition->bRedirectableProjectile)
	{
		return false;
	}

	if (!IsValid(CastingActor) || !IsValid(GetWorld()))
	{
		return false;
	}

	APawn* CastingPawn = Cast<APawn>(CastingActor);

	if (!IsValid(CastingPawn))
	{
		return false;
	}

	APlayerController* PlayerController = Cast<APlayerController>(CastingPawn->GetController());

	if (!IsValid(PlayerController))
	{
		return false;
	}

	FVector ViewLocation = FVector::ZeroVector;
	FRotator ViewRotation = FRotator::ZeroRotator;

	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

	const float TraceDistance = FMath::Max(
		SpellDefinition->RedirectAimTraceDistance,
		100.0f
	);

	const FVector TraceEnd =
		ViewLocation +
		ViewRotation.Vector() *
		TraceDistance;

	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = true;
	TraceParams.AddIgnoredActor(CastingActor);
	TraceParams.AddIgnoredActor(this);

	FHitResult AimHit;

	const bool bHitSomething = GetWorld()->LineTraceSingleByChannel(
		AimHit,
		ViewLocation,
		TraceEnd,
		ECC_Visibility,
		TraceParams
	);

	OutAimPoint = bHitSomething
		? AimHit.ImpactPoint
		: TraceEnd;

	return true;
}

void ASpellProjectile::ResumePreviousRedirectDirection()
{
	if (!bPausedForRedirect)
	{
		return;
	}

	FVector TravelDirection = PreRedirectVelocity;

	if (TravelDirection.IsNearlyZero())
	{
		TravelDirection = GetActorForwardVector();
	}

	TravelDirection.Normalize();

	ProjectileMovement->Activate(true);
	ProjectileMovement->Velocity = TravelDirection * RedirectProjectileSpeed;
	ProjectileMovement->UpdateComponentVelocity();

	bPausedForRedirect = false;
	PreRedirectVelocity = ProjectileMovement->Velocity;

	if (SpellDefinition && SpellDefinition->bProjectileHoming)
	{
		ProjectileMovement->HomingAccelerationMagnitude = SpellDefinition->ProjectileHomingStrength;
		AcquireHomingTarget();
	}
}

void ASpellProjectile::ProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!SpellDefinition)
	{
		return;
	}

	if (!OtherActor)
	{
		return;
	}

	if (OtherActor == CastingActor)
	{
		return;
	}

	if (!SpellDefinition->bProjectilePierces)
	{
		return;
	}

	if (SpellDefinition->bProjectileBounces)
	{
		return;
	}

	const bool bNewEnemyHit = ApplyImpactDamageOnce(OtherActor);

	if (!bNewEnemyHit)
	{
		return;
	}

	CurrentPierceCount++;

	if (SpellDefinition->bModifyDamagePerPierce)
	{
		CurrentImpactDamage *= SpellDefinition->DamageMultiplierPerPierce;
	}

	if (
		SpellDefinition->bLimitProjectilePierces &&
		CurrentPierceCount >= SpellDefinition->MaximumProjectilePierces
		)
	{
		CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}
}

void ASpellProjectile::ProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor)
	{
		Destroy();
		return;
	}

	if (OtherActor == CastingActor)
	{
		return;
	}

	if (!SpellDefinition)
	{
		Destroy();
		return;
	}

	ApplyImpactDamageOnce(OtherActor);

	if (SpellDefinition->bCreatesGravity && !bGravityActive)
	{
		if (SpellDefinition->GravityActivationMode == EGravityActivationMode::OnImpact)
		{
			ActivateGravity();
		}
		else
		{
			StopForGravityAnchor();
		}

		return;
	}

	if (
		SpellDefinition->bProjectileBounces &&
		CurrentBounceCount < SpellDefinition->MaximumProjectileBounces
		)
	{
		CurrentBounceCount++;

		return;
	}

	if (SpellDefinition->ProducesLingeringField())
	{
		FVector FieldLocation = CollisionSphere->GetComponentLocation();

		if (Hit.bBlockingHit)
		{
			const float MaximumImpactDistance = FMath::Max(
				CollisionSphere->GetScaledSphereRadius() * 2.0f,
				100.0f
			);

			if (
				FVector::DistSquared(
					Hit.ImpactPoint,
					FieldLocation
				) <=
				FMath::Square(MaximumImpactDistance)
				)
			{
				FieldLocation = Hit.ImpactPoint;
			}
		}

		CreateLingeringField(FieldLocation);
	}

	Destroy();
}

bool ASpellProjectile::ApplyImpactDamageOnce(AActor* OtherActor)
{
	if (!SpellDefinition)
	{
		return false;
	}

	if (!OtherActor)
	{
		return false;
	}

	if (DamagedActors.Contains(OtherActor))
	{
		return false;
	}

	if (AEnemy* EnemyCharacter = Cast<AEnemy>(OtherActor))
	{
		DamagedActors.Add(OtherActor);

		if (SpellDefinition->bInflictsImpactDamage)
		{
			AController* DamageInstigator = nullptr;

			if (APawn* CastingPawn = Cast<APawn>(CastingActor))
			{
				DamageInstigator = CastingPawn->GetController();
			}

			UGameplayStatics::ApplyDamage(
				EnemyCharacter,
				CurrentImpactDamage * CurrentScaleDamageMultiplier,
				DamageInstigator,
				this,
				nullptr
			);
		}

		if (SpellDefinition->bProjectileHoming)
		{
			AcquireHomingTarget();
		}

		return true;
	}

	if (ABaseCharacter* PlayerCharacter = Cast<ABaseCharacter>(OtherActor))
	{
		//PlayerCharacter->ApplyDamage(SpellDefinition->ImpactDamageStrength);
	}

	return false;
}

void ASpellProjectile::AcquireHomingTarget()
{
	if (!SpellDefinition)
	{
		return;
	}

	if (!SpellDefinition->bProjectileHoming)
	{
		return;
	}

	ProjectileMovement->bIsHomingProjectile = false;
	ProjectileMovement->HomingTargetComponent.Reset();

	TArray<AActor*> EnemyActors;

	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		AEnemy::StaticClass(),
		EnemyActors
	);

	AActor* ClosestEnemy = nullptr;

	float ClosestDistanceSquared = FMath::Square(SpellDefinition->ProjectileHomingDetectionRadius);

	for (AActor* EnemyActor : EnemyActors)
	{
		if (!IsValid(EnemyActor))
		{
			continue;
		}

		if (DamagedActors.Contains(EnemyActor))
		{
			continue;
		}

		const float DistanceSquared = FVector::DistSquared(
			GetActorLocation(),
			EnemyActor->GetActorLocation()
		);

		if (DistanceSquared > ClosestDistanceSquared)
		{
			continue;
		}

		ClosestEnemy = EnemyActor;
		ClosestDistanceSquared = DistanceSquared;
	}

	if (!IsValid(ClosestEnemy))
	{
		return;
	}

	USceneComponent* TargetComponent = ClosestEnemy->GetRootComponent();

	if (!IsValid(TargetComponent))
	{
		return;
	}

	ProjectileMovement->HomingTargetComponent = TargetComponent;
	ProjectileMovement->bIsHomingProjectile = true;
}

void ASpellProjectile::CreateLingeringField(const FVector& FieldLocation)
{
	if (!SpellDefinition)
	{
		return;
	}

	if (!SpellDefinition->ProducesLingeringField())
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;

	SpawnParameters.Owner = CastingActor;
	SpawnParameters.Instigator = Cast<APawn>(CastingActor);
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ASpellLingeringField* LingeringField = GetWorld()->SpawnActor<ASpellLingeringField>(
		ASpellLingeringField::StaticClass(),
		FieldLocation,
		FRotator::ZeroRotator,
		SpawnParameters
	);

	if (!IsValid(LingeringField))
	{
		return;
	}

	LingeringField->bShowLingeringHitbox = bShowLingeringHitbox;

	LingeringField->InitializeLingeringField(
		CastingActor,
		SpellDefinition
	);
}