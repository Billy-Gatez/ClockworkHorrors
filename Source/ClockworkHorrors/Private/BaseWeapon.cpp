// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#include "BaseWeapon.h"

#include "BaseCharacter.h"
#include "Components/CharacterAnimationComponent.h"
#include "GameFramework/Character.h"
#include "WeaponPickup.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Utils/InventoryItemDataAsset.h"
#include <Utils/PlayerSaveGame.h>
#include "Utils/SkillTreeComponent.h"
#include "Utils/SkillTreeDataAsset.h"
#include "UI/MainInventoryWidget.h"
#include"Utils/InventoryComponent.h"
// Sets default values
ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	skeletalMesh =
		CreateDefaultSubobject<USkeletalMeshComponent>("skeletalMesh");

	SetRootComponent(skeletalMesh);
	skeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ActionHappening = false;
	Alive = true;

	AnimationStance = ECharacterAnimationStance::Unarmed;

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>("InteractWidgetComponent");
	InteractRangeSphere = CreateDefaultSubobject<USphereComponent>("InteractSphereComponent");
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	UWeaponPickup* WeaponPickup = FindComponentByClass<UWeaponPickup>();

	if (IsValid(WeaponPickup))
	{
		WeaponPickup->OnWeaponEquippedStateChanged.AddDynamic(
			this,
			&ABaseWeapon::HandleWeaponEquippedStateChanged
		);
	}

	if (InteractWidget)
	{
		InteractWidget->SetVisibility(false);
	}

	if( InteractRangeSphere)
	{
		InteractRangeSphere->OnComponentBeginOverlap.AddDynamic(
			this,
			&ABaseWeapon::HandleOverlapBegin
		);
		InteractRangeSphere->OnComponentEndOverlap.AddDynamic(
			this,
			&ABaseWeapon::HandleOverlapEnd
		);
	}
}

const bool ABaseWeapon::CanAttack()
{
	return !ActionHappening && Alive;
}

void ABaseWeapon::Attack()
{
	if (!ParentPawn)
	{
		UWeaponPickup* Pickup = FindComponentByClass<UWeaponPickup>();

		if (IsValid(Pickup) && Pickup->IsEquipped())
		{
			ParentPawn = Pickup->GetEquippedHolder();
		}
	}

	if (!ParentPawn)
	{
		return;
	}
}

void ABaseWeapon::ActionStopped()
{
	ActionHappening = false;
	OnActionDelegateInstance.Broadcast();
}

float ABaseWeapon::GetDamage()
{
	return damage;
}

void ABaseWeapon::CalculateNewDamage()
{
	UWeaponPickup* Pickup = FindComponentByClass<UWeaponPickup>();
	float BaseDamage = Pickup->ItemDataAsset->DamageAmount;
	ABaseCharacter* Character = Cast<ABaseCharacter>(ParentPawn);
	USkillTreeComponent* skillTree = Character->GetComponentByClass<USkillTreeComponent>();
	if (Character->InventoryWidget)
	{
		//1 = sword 2 = staff 3 = rifle
		if (Character->InventoryWidget->SkillTreePath == 1)
		{
			if (Pickup->ItemDataAsset->WeaponType == "Sword")
			{
				for (size_t i = 0; i < Character->InventoryWidget->skillSelection.Len(); i++)
				{
					if (Character->InventoryWidget->skillSelection[i] == '1') {
						BaseDamage += skillTree->SwordSkills[i]->damage;
					}
				}
			}
		}
		else if (Character->InventoryWidget->SkillTreePath == 2)
		{
			if (Pickup->ItemDataAsset->WeaponType == "Staff")
			{

			}
		}
		else if (Character->InventoryWidget->SkillTreePath == 3)
		{
			if (Pickup->ItemDataAsset->WeaponType == "Blaster")
			{

			}
		}
	}
	damage = BaseDamage;
}

FVector ABaseWeapon::GetSource()
{
	return skeletalMesh->GetSocketLocation(WeaponSocket);
}

void ABaseWeapon::OwnerDied()
{
	Alive = false;
}

void ABaseWeapon::HandleWeaponEquippedStateChanged(bool bWeaponEquipped)
{
	UWeaponPickup* Pickup = FindComponentByClass<UWeaponPickup>();

	// Cache the holder while the pickup still knows who owns it.
	if (bWeaponEquipped && IsValid(Pickup))
	{
		if (ACharacter* EquippedHolder = Pickup->GetEquippedHolder())
		{
			ParentPawn = EquippedHolder;
		}
	}

	InteractWidget->SetVisibility(false);

	if (!ParentPawn)
	{
		return;
	}

	// Animation stance is universal: any pawn with the reusable
	// animation component can respond to the equipped weapon.
	if (
		UCharacterAnimationComponent* AnimationComponent =
		ParentPawn->FindComponentByClass<UCharacterAnimationComponent>()
		)
	{
		if (bWeaponEquipped)
		{
			AnimationComponent->SetAnimationStance(AnimationStance);
		}
		else
		{
			AnimationComponent->ResetAnimationStance();
		}
	}

	// Preserve the project's existing BaseCharacter weapon pointer flow.
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(ParentPawn))
	{
		if (bWeaponEquipped)
		{
			Character->weapon = this;
			CalculateNewDamage();

			if (Pickup->ItemDataAsset->AmmoType && Character->bFinishedBeginPlay)
			{
				FInventorySlotEntry slot = Character->InventoryComponent->GetItem( Pickup->ItemDataAsset->ItemName);
				if (slot.Ammo >= 0 && Pickup->ItemDataAsset->AmmoType)
				{
					
					Character->OnAmmoChanged.Broadcast(slot.Ammo, Pickup->ItemDataAsset->MaxAmmo, true);
				}
				else
				{
					Character->OnAmmoChanged.Broadcast(0, 0, false);
				}
				
			}
			else
			{
				Character->OnAmmoChanged.Broadcast(0, 0, false);
			}
		}
		else if (Character->weapon == this)
		{
			Character->weapon = nullptr;
			Character->OnAmmoChanged.Broadcast(0, 0, false);
		}
	}
}

void ABaseWeapon::HandleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor || OtherActor == this)
	{
		return;
	}

	if(!InteractWidget)
	{
		return;
	}

	if(bPickedUp)
	{
		InteractWidget->SetVisibility(false);
		return;
	}

	if(ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor))
	{
		InteractWidget->SetVisibility(true);
	}
}

void ABaseWeapon::HandleOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(!OtherActor || OtherActor == this)
	{
		return;
	}
	if(!InteractWidget)
	{
		return;
	}
	if(ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor))
	{
		InteractWidget->SetVisibility(false);
	}
}
