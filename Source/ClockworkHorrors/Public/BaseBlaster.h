// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "BaseBlaster.generated.h"


UCLASS()
class CLOCKWORKHORRORS_API ABaseBlaster : public ABaseWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseBlaster();
	int currentAmmo;
	int MaxAmmo;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;




	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> projectileClass;


	void useAmmo();
	class AProjectile* projectile;
	virtual const bool CanAttack() override;
public:
	virtual void Attack() override;

	UPROPERTY(BlueprintAssignable, Category = "Blaster")
	FWeaponDelegate OnReloadStart;
	UFUNCTION()
	void reloadAmmo();
private:
	struct FInventorySlotEntry GetWeaponSlot();
	struct FInventorySlotEntry GetAmmoSlot();

};