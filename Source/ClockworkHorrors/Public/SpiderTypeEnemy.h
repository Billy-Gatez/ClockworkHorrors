// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "SpiderTypeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CLOCKWORKHORRORS_API ASpiderTypeEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	ASpiderTypeEnemy();

protected:
	virtual void PerformAttack() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spider")
	class USphereComponent* TrueCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spider | Status Effect")
	float stunTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spider | Status Effect")
	float SlowPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spider | Status Effect")
	float PoisonDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spider | Status Effect")
	float PoisonTickDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spider | Status Effect")
	float PoisonTickInterval;

private:
	int32 AttackTypeInt;

public:
	//virtual void Tick(float DeltaTime) override;

	virtual UStatusEffectType* GetStatusEffectPayload(AActor* Target = nullptr) const override;
	
};
