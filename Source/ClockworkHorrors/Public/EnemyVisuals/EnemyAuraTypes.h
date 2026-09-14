#pragma once

#include "CoreMinimal.h"
#include "EnemyAuraTypes.generated.h"

/**
 * Visual aura categories used to identify Clockwork Horrors enemy types.
 *
 * These values intentionally match the enemy type classes that already exist
 * in the project. The enum is kept separate from the enemy hierarchy so the
 * aura system can be tested without modifying production enemy classes.
 */
UENUM(BlueprintType)
enum class EEnemyAuraType : uint8
{
	None	UMETA(DisplayName = "None"),
	Normal	UMETA(DisplayName = "Normal"),
	Fire	UMETA(DisplayName = "Fire"),
	Ice		UMETA(DisplayName = "Ice"),
	Poison	UMETA(DisplayName = "Poison"),
	Heavy	UMETA(DisplayName = "Heavy")
};
