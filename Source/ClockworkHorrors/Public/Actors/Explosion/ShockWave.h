// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/StatusEffectSource.h"
#include "ShockWave.generated.h"

UCLASS()
class CLOCKWORKHORRORS_API AShockWave : public AActor, public IStatusEffectSource
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShockWave();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShockWave")
	class USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShockWave")
	class UStaticMeshComponent* SphereMesh;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShockWave")
	float ShockWaveRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShockWave")
	float ShockWaveForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShockWave")
	float ShockWaveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShockWave")
	float ShockWaveDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShockWave")
	float StunDuration;

	float CurrentRadius;

	UPROPERTY()
	TSet<AActor*> HitActors;

	UFUNCTION()
	void OnSphereBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual UStatusEffectType* GetStatusEffectPayload(AActor* TargetActor = nullptr) const override;

};
