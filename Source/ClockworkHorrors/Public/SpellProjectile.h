// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UPrimitiveComponent;
class UManaComponent;
class ASpells;
class ACharacter;
class AEnemy;

UCLASS(Blueprintable)
class CLOCKWORKHORRORS_API ASpellProjectile : public AActor
{
	GENERATED_BODY()

public:
	ASpellProjectile();

	virtual void Tick(float DeltaTime) override;

	void InitializeProjectile(AActor* InCastingActor, ASpells* InSpellDefinition, const FVector& InTravelDirection);

	void InitializeProjectile(AActor* InCastingActor, ASpells* InSpellDefinition, const FVector& InTravelDirection, float InChargePercent);

	UFUNCTION(BlueprintCallable, Category = "Projectile|Redirect")
	bool PauseForRedirect();

	UFUNCTION(BlueprintCallable, Category = "Projectile|Redirect")
	bool RedirectProjectile(const FVector& NewTravelDirection);

	UFUNCTION(BlueprintPure, Category = "Projectile|Redirect")
	bool IsPausedForRedirect() const;

	UFUNCTION(BlueprintCallable, Category = "Projectile|Gravity")
	bool ActivateGravity();

	UFUNCTION(BlueprintPure, Category = "Projectile|Gravity")
	bool IsGravityActive() const;

	bool GetRedirectAimPoint(FVector& OutAimPoint) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UStaticMeshComponent* ProjectileAppearance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Debug")
	bool bShowProjectileHitbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Debug")
	bool bShowLingeringHitbox;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void ProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void ProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY()
	AActor* CastingActor;

	UPROPERTY()
	ASpells* SpellDefinition;

	UPROPERTY()
	UPrimitiveComponent* CastingCollisionComponent;

	UPROPERTY()
	UManaComponent* CasterManaComponent;

	UPROPERTY()
	TArray<AActor*> DamagedActors;

	int32 CurrentBounceCount;
	int32 CurrentPierceCount;

	float CurrentImpactDamage;
	float ChargePercent;

	float ScaleChangeElapsedSeconds;
	float BaseProjectileCollisionRadius;
	float CurrentScaleDamageMultiplier;
	FVector BaseProjectileAppearanceScale;

	bool bPausedForRedirect;
	bool bManaRecoveryBlockApplied;
	float RedirectProjectileSpeed;
	FVector PreRedirectVelocity;

	UPROPERTY()
	ACharacter* RiderCharacter;

	bool bRiderAttached;
	bool bSavedImpartBaseVelocityX;
	bool bSavedImpartBaseVelocityY;
	bool bSavedImpartBaseVelocityZ;
	bool bSavedImpartBaseAngularVelocity;
	bool bSavedIgnoreBaseRotation;
	FVector ProjectileAppearanceBaseRelativeLocation;
	FRotator ProjectileAppearanceBaseRelativeRotation;

	bool bGravityActive;
	bool bGravityProjectileAnchored;
	float GravityActivationDelayRemaining;
	TMap<TWeakObjectPtr<AEnemy>, uint8> GravityLockedEnemyMovementModes;
	TMap<TWeakObjectPtr<AEnemy>, bool> GravityAffectedEnemyTickStates;

	void UpdateProjectileRider();
	void AttachProjectileRider(ACharacter* Character);
	void DetachProjectileRider();

	void ResumePreviousRedirectDirection();

	void UpdateProjectileScaleOverTime(float DeltaTime);

	void UpdateGravity(float DeltaTime);
	void StopForGravityAnchor();
	void DisableEnemyActionsForGravity(AEnemy* EnemyCharacter);
	void LockEnemyInGravity(AEnemy* EnemyCharacter);
	void ReleaseGravityAffectedEnemies();

	void CreateLingeringField(const FVector& FieldLocation);

	bool ApplyImpactDamageOnce(AActor* OtherActor);

	void AcquireHomingTarget();
};