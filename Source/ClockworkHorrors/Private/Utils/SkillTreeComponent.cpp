// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.


#include "Utils/SkillTreeComponent.h"
#include "Utils/SkillTreeDataAsset.h"
#include "BaseCharacter.h"
#include "Utils/HealthComponent.h"
#include "Utils/InventoryItemDataAsset.h"
#include "BaseWeapon.h"
#include <WeaponPickup.h>
// Sets default values for this component's properties
USkillTreeComponent::USkillTreeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USkillTreeComponent::BeginPlay()
{
	Super::BeginPlay();
	SwordSkills =
	{
		SwordSkill1,
		SwordSkill2,
		SwordSkill3,
		SwordSkill4,
		SwordSkill5,
		SwordSkill6,
		SwordSkill7,
		SwordSkill8,
		SwordSkill9,
		SwordSkill10
	};
	StaffSkills =
	{
		StaffSkill1,
		StaffSkill2,
		StaffSkill3,
		StaffSkill4,
		StaffSkill5,
		StaffSkill6,
		StaffSkill7,
		StaffSkill8,
		StaffSkill9,
		StaffSkill10
	};
	BlasterSkills =
	{
		BlasterSkill1,
		BlasterSkill2,
		BlasterSkill3,
		BlasterSkill4,
		BlasterSkill5,
		BlasterSkill6,
		BlasterSkill7,
		BlasterSkill8,
		BlasterSkill9,
		BlasterSkill10
	};
	// ...
	
}

void USkillTreeComponent::SkillUpgraded(int32 SkillTreePath, int32 SkillIndex)
{
	USkillTreeDataAsset* skill = nullptr;
	switch (SkillTreePath)
	{
	case 1:
	{
		skill = SwordSkills[SkillIndex];
		break;
	}
	case 2:
	{
		skill = StaffSkills[SkillIndex];
		break;
	}
	case 3:
	{
		skill = BlasterSkills[SkillIndex];
		break;
	}
	default:
		break;
	}
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		ABaseCharacter* player = Cast<ABaseCharacter>(PlayerController->GetPawn());
		if (player)
		{
			if (skill->bIsStatUpgrade)
			{
				UHealthComponent* health = player->GetComponentByClass<UHealthComponent>();
				health->SetMaxHealth(health->GetMaxHealth() + skill->Hp);
				if (skill->WeaponToModify)
				{
					if (player->weapon)
					{
						UWeaponPickup* pickup = player->weapon->GetComponentByClass<UWeaponPickup>();
							if (pickup)
							{
								if (pickup->ItemDataAsset->WeaponType == skill->WeaponToModify->WeaponType)
								{
								player->weapon->SetDamage(player->weapon->GetDamage() + skill->damage);
								}
					}
					}
					
				}
			}
		}
	}

}




