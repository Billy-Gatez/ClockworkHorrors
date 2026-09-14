// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.


#include "Utils/SkillTreeDataAsset.h"

USkillTreeDataAsset::USkillTreeDataAsset()
{
	bIsStatUpgrade = false;
	bIsAbilityUpgrade = false;
	bIsComponent = false;
	Hp = 0;
	damage = 0;
	UpgradeComponent = nullptr;
	WeaponToModify = nullptr;
}
