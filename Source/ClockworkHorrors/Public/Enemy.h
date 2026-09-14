// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/EnemyInterface.h"
#include "Interfaces/StatusEffectSource.h"
#include "Engine/DamageEvents.h"
#include "Enemy.generated.h"


class UHealthComponent;
class UInputComponent;
class UTargetableComponent;
class UEnemyAuraComponent;
class UWidgetComponent;
class UUserWidget;


UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Patrolling UMETA(DisplayName = "Patrolling"),
	Chasing   UMETA(DisplayName = "Chasing"),
	Attacking UMETA(DisplayName = "Attacking"),
	Dead      UMETA(DisplayName = "Dead")
};


UCLASS(Abstract)
class CLOCKWORKHORRORS_API AEnemy : public ACharacter, public IEnemyInterface, public IStatusEffectSource
{
	GENERATED_BODY()


public:

	AEnemy();


	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(
		UInputComponent* PlayerInputComponent
	) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy|Combat")
	float GetExperienceReward() const { return ExperienceReward; }


protected:

	virtual void BeginPlay() override;


	// =========================================================
	// COMPONENTS
	// =========================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> HealthBarComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UFUNCTION()
	void UpdateHealthBar();

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Components"
	)
	UHealthComponent* HealthComponent;


	/**
	 * Marks this enemy as a valid Z-lock target.
	 *
	 * This component is created on AEnemy so every Blueprint/C++ child enemy
	 * automatically participates in the same targeting system. Child Blueprints
	 * can still tune Target Offset, Target Priority, and Can Be Targeted.
	 */
	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Components"
	)
	UTargetableComponent* TargetableComponent;


	/**
	 * Shared lock-on indicator for every enemy derived from AEnemy.
	 *
	 * The default widget, screen-space behavior, draw size, and offset are
	 * configured in C++. Child Blueprints can still override the inherited
	 * Widget Component settings for unusually small/large enemies or bosses.
	 */
	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Components"
	)
	UWidgetComponent* TargetLockIndicator;



	/**
	 * Runtime visual identifier for this enemy's type.
	 *
	 * Created once on AEnemy so every C++/Blueprint child automatically owns
	 * the same aura system. Child enemy types only choose their AuraType.
	 * The component also contains a master Aura Enabled switch so the visual
	 * can be disabled without removing the production integration.
	 */
	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Components"
	)
	UEnemyAuraComponent* EnemyAuraComponent;



	// =========================================================
	// COMBAT
	// =========================================================

	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "Enemy|Combat"
	)
	float AttackDamage = 0.2f;


	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "Enemy|Combat"
	)
	float AttackRange = 150.0f;


	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "Enemy|Combat"
	)
	float AttackCooldown = 0.5f;


	// =========================================================
	// AI / MOVEMENT
	// =========================================================

	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "Enemy|AI"
	)
	float MoveSpeed = 250.0f;


	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "Enemy|AI"
	)
	float PatrolRadius = 800.0f;


	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "Enemy|AI"
	)
	float DetectionRadius = 600.0f;


	// =========================================================
	// DEATH
	// =========================================================

	/**
	 * How long the enemy remains in the world after dying.
	 *
	 * 0 = destroy immediately.
	 *
	 * Later you can set this to something like 3 seconds
	 * if you want a death animation or ragdoll.
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "Enemy|Death",
		meta = (ClampMin = "0.0")
	)
	float DeathDestroyDelay = 0.0f;

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Enemy|Death"
	)
	float ExperienceReward;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "Enemy|Death"
	)
	TSubclassOf<AActor> LootDropClass;


	// =========================================================
	// STATE
	// =========================================================

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Enemy|State"
	)
	bool bIsAttacking = false;


	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Enemy|State"
	)
	bool bCanDealDamage = false;


	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Enemy|State"
	)
	EEnemyState CurrentState =
		EEnemyState::Patrolling;


	// =========================================================
	// TARGETING
	// =========================================================

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Enemy|AI"
	)
	AActor* TargetActor = nullptr;


	// =========================================================
	// PATROL
	// =========================================================

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Enemy|AI"
	)
	FVector SpawnLocation;


	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Enemy|AI"
	)
	FVector PatrolDestination;


	// =========================================================
	// INTERNAL
	// =========================================================

	float AttackTimer = 0.0f;


	// =========================================================
	// SHARED ENEMY BEHAVIOR
	// =========================================================

	virtual void AcquireTarget();


	virtual void UpdateState(
		float DeltaTime
	);


	virtual void HandlePatrol(
		float DeltaTime
	);


	virtual void HandleChase(
		float DeltaTime
	);


	virtual void HandleAttack(
		float DeltaTime
	);


	virtual void ChooseNewPatrolDestination();


	/**
	 * Called when this enemy's attack cooldown finishes.
	 *
	 * Child enemy classes should override this when they
	 * need a different attack.
	 */
	virtual void PerformAttack() override;

	virtual void LandFromJump() override;

	UFUNCTION()
	void HandleActionFinished();

	FTimerHandle AttackResetTimerHandle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* sound;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	USoundAttenuation* soundAttenuation;
	UFUNCTION()
	void HandleHurt();

	/** Keep the shared indicator synced with TargetableComponent state. */
	UFUNCTION()
	void HandleTargetedStateChanged(bool bIsTargeted);

	// =========================================================
	// DEATH
	// =========================================================

	/**
	 * Called automatically when HealthComponent reaches zero.
	 */
	UFUNCTION()
	void HandleDeath();


	/**
	 * Blueprint hook for death effects.
	 *
	 * BP children can implement this for:
	 * - animation
	 * - particles
	 * - sound
	 * - ragdoll
	 * etc.
	 */
	UFUNCTION(
		BlueprintImplementableEvent,
		Category = "Enemy|Events"
	)
	void OnDeath(); 

	virtual UStatusEffectType* GetStatusEffectPayload(AActor* Target = nullptr) const override;
};