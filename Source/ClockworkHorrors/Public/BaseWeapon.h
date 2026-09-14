// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Animation/CharacterAnimationSet.h"
#include "BaseWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponDelegate);

UCLASS()
class CLOCKWORKHORRORS_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName WeaponSocket;

	/**
	 * Persistent animation posture this weapon requests while equipped.
	 * Child weapon Blueprints/classes can override this without needing
	 * new animation state-machine states.
	 */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadWrite,
		Category = "Weapon|Animation"
	)
	ECharacterAnimationStance AnimationStance =
		ECharacterAnimationStance::Unarmed;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USkeletalMeshComponent* skeletalMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UStaticMeshComponent* staticMesh;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	APawn* ParentPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	class USphereComponent* InteractRangeSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	float InteractRange = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	class UWidgetComponent* InteractWidget;
	void SetDamage(float NewDamage) { damage = NewDamage; }
protected:
	virtual void BeginPlay() override;

	virtual const bool CanAttack();
	FVector GetSource();
	void OwnerDied();

	UFUNCTION()
	void HandleWeaponEquippedStateChanged(bool bWeaponEquipped);

	UFUNCTION()
	void HandleOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void HandleOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float damage = 10.0f;

	bool Alive;

public:
	virtual void Attack();

	FWeaponDelegate OnDelegateInstance;
	FWeaponDelegate OnActionDelegateInstance;

	UFUNCTION()
	void ActionStopped();

	float GetDamage();

	bool bPickedUp;
	bool ActionHappening;
	void CalculateNewDamage();
};
