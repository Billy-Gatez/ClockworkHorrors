// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CLOCKWORKHORRORS_API UPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
	float currentHealth;
	UPROPERTY()
	float MaxExperience;
	UPROPERTY()
	float Experience;
	UPROPERTY()
	int32 Level;
	UPROPERTY()
	int32 SkillPoints;
	UPROPERTY()
	TArray<struct FInventorySlotEntry> Inventory;
	UPROPERTY()
	int32 currentEquippedSlot;
	UPROPERTY()
	int32 SkillTreePath = 0;
	UPROPERTY()
	FString SkillTreeSelections = "0000000000";
	bool SavePlayerData(float health,float maxExp, float exp, int32 curLevel, int32 CurrentSkillPoints, TArray<struct FInventorySlotEntry> curInventory, int32 equippedSlot, int32 SkillPath, FString SkillSelections);
	static UPlayerSaveGame* LoadPlayerData();
};
