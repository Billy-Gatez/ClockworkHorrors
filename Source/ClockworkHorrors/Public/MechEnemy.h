// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "MechEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CLOCKWORKHORRORS_API AMechEnemy : public AEnemy
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShockWave")
	TSubclassOf<AActor> ShockWave;


public:
	virtual void LandFromJump() override;

	virtual UStatusEffectType* GetStatusEffectPayload(AActor* Target = nullptr) const override;
	
};
