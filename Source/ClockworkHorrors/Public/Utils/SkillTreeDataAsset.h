// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillTreeDataAsset.generated.h"


/**
 * 
 */
UCLASS()
class CLOCKWORKHORRORS_API USkillTreeDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	USkillTreeDataAsset();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgrade|Flags")
	bool bIsStatUpgrade;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgrade|Flags")
	bool bIsAbilityUpgrade;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgrade|Flags")
	bool bIsComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgrade|Stats", meta = (EditCondition = "bIsStatUpgrade"))
	float Hp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgrade|Stats", meta = (EditCondition = "bIsStatUpgrade"))
	float damage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgrade|Stats", meta = (EditCondition = "bIsStatUpgrade"))
	class UInventoryItemDataAsset* WeaponToModify;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgrade|Stats", meta = (EditCondition = "bIsStatUpgrade"))
	FName WeaponType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgrade|Ability|Component", meta = (EditCondition = "bIsComponent"))
	TSubclassOf< UActorComponent>  UpgradeComponent;
	

};
