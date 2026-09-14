// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ManaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnManaChanged, float, CurrentMana, float, MaxMana);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CLOCKWORKHORRORS_API UManaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UManaComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
	float MaxMana = 100.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Mana")
	float CurrentMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
	float ManaRegenRate = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
	bool bCanRegenerateMana = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana", meta = (EditCondition = "bCanRegenerateMana", EditConditionHides, ClampMin = "0.0"))
	float ManaRegenDelaySeconds = 1.5f;

	UPROPERTY(BlueprintAssignable, Category = "Mana")
	FOnManaChanged OnManaChanged;

	UFUNCTION(BlueprintCallable, Category = "Mana")
	bool ConsumeMana(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Mana")
	void RegenerateMana(float Amount);

	UFUNCTION(BlueprintPure, Category = "Mana")
	bool HasEnoughMana(float Amount) const;

	UFUNCTION(BlueprintPure, Category = "Mana")
	float GetManaPercentage() const;

	UFUNCTION(BlueprintPure, Category = "Mana")
	float GetCurrentMana() const;

	UFUNCTION(BlueprintPure, Category = "Mana")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "Mana")
	void SetCurrentMana(float NewCurrentMana);

	UFUNCTION(BlueprintCallable, Category = "Mana")
	void SetMaxMana(float NewMaxMana, bool bFillMana);

	UFUNCTION(BlueprintCallable, Category = "Mana")
	void AddManaRecoveryBlock();

	UFUNCTION(BlueprintCallable, Category = "Mana")
	void RemoveManaRecoveryBlock();

	UFUNCTION(BlueprintPure, Category = "Mana")
	bool IsManaRecoveryBlocked() const;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float ManaRegenDelayRemaining = 0.0f;
	int32 ManaRecoveryBlockCount = 0;
};
