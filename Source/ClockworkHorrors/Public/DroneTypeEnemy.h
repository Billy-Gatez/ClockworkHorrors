// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "DroneTypeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CLOCKWORKHORRORS_API ADroneTypeEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	ADroneTypeEnemy();

protected:
	virtual void BeginPlay() override;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Movement")
	float HoverHeight = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Movement")
	float HoverSpeed = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Movement")
	float HoverAmplitude = 20.0f;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	class UBehaviorTree* DroneBehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	class USphereComponent* TrueCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Movement")
	class UStaticMeshComponent* DroneMesh;

	UPROPERTY(EditAnywhere, Category = "Drone")
	float HoverHeight = 150.f;

public:
	virtual void Tick(float DeltaTime) override;
	
};
