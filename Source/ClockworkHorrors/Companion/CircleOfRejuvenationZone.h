#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CircleOfRejuvenationZone.generated.h"

class USphereComponent;
class UNiagaraComponent;

UCLASS()
class CLOCKWORKHORRORS_API ACircleOfRejuvenationZone : public AActor
{
	GENERATED_BODY()

public:
	ACircleOfRejuvenationZone();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// --- COMPONENTS ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* OverlapSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* ZoneNiagaraFX;

	// --- CONFIGURATION ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Settings")
	float ZoneRadius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Settings")
	float Duration = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Settings")
	float HealInterval = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Settings")
	float HealPerTick = 0.5f;

private:
	FTimerHandle HealTimerHandle;
	FTimerHandle ExpirationTimerHandle;

	void PerformPulseHeal();
	void ExpireZone();
};