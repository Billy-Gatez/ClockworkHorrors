#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "EnemyVisuals/EnemyAuraTypes.h"
#include "EnemyAuraComponent.generated.h"

struct FPropertyChangedEvent;

/**
 * Reusable Niagara component that visually identifies an enemy type.
 *
 * The assigned Niagara System should expose:
 *   User.AuraColor      (Linear Color)
 *   User.AuraIntensity  (Float)
 *   User.AuraScale      (Float)
 */
UCLASS(
	ClassGroup = (ClockworkHorrors),
	meta = (BlueprintSpawnableComponent),
	DisplayName = "Enemy Aura Component"
)
class CLOCKWORKHORRORS_API UEnemyAuraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UEnemyAuraComponent();

	UFUNCTION(BlueprintCallable, Category = "Enemy Aura")
	void SetAuraType(EEnemyAuraType NewAuraType);

	UFUNCTION(BlueprintPure, Category = "Enemy Aura")
	EEnemyAuraType GetAuraType() const
	{
		return AuraType;
	}

	UFUNCTION(BlueprintCallable, Category = "Enemy Aura")
	void SetAuraEnabled(bool bEnabled);

	UFUNCTION(BlueprintPure, Category = "Enemy Aura")
	bool IsAuraEnabled() const
	{
		return bAuraEnabled;
	}

	UFUNCTION(BlueprintCallable, Category = "Enemy Aura")
	void RefreshAura();

protected:
	virtual void BeginPlay() override;
	virtual void OnRegister() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(
		FPropertyChangedEvent& PropertyChangedEvent
	) override;
#endif

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Enemy Aura|Type"
	)
	bool bAuraEnabled = true;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Enemy Aura|Type"
	)
	EEnemyAuraType AuraType = EEnemyAuraType::None;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Enemy Aura|Type"
	)
	bool bShowNormalAura = false;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Enemy Aura|Overrides"
	)
	bool bOverrideAuraColor = false;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Enemy Aura|Overrides",
		meta = (EditCondition = "bOverrideAuraColor", EditConditionHides)
	)
	FLinearColor AuraColorOverride = FLinearColor::White;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Enemy Aura|Overrides"
	)
	bool bOverrideAuraIntensity = false;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Enemy Aura|Overrides",
		meta = (
			EditCondition = "bOverrideAuraIntensity",
			EditConditionHides,
			ClampMin = "0.0",
			UIMin = "0.0",
			UIMax = "5.0"
		)
	)
	float AuraIntensityOverride = 1.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Enemy Aura|Overrides"
	)
	bool bOverrideAuraScale = false;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Enemy Aura|Overrides",
		meta = (
			EditCondition = "bOverrideAuraScale",
			EditConditionHides,
			ClampMin = "0.0",
			UIMin = "0.0",
			UIMax = "3.0"
		)
	)
	float AuraScaleOverride = 1.0f;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		AdvancedDisplay,
		Category = "Enemy Aura|Niagara Parameters"
	)
	FName AuraColorParameterName = TEXT("User.AuraColor");

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		AdvancedDisplay,
		Category = "Enemy Aura|Niagara Parameters"
	)
	FName AuraIntensityParameterName = TEXT("User.AuraIntensity");

	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		AdvancedDisplay,
		Category = "Enemy Aura|Niagara Parameters"
	)
	FName AuraScaleParameterName = TEXT("User.AuraScale");

private:
	void ApplyAuraSettings();

	bool ShouldAuraBeVisible() const;

	FLinearColor GetResolvedAuraColor() const;
	float GetResolvedAuraIntensity() const;
	float GetResolvedAuraScale() const;

	FLinearColor GetDefaultAuraColor() const;
	float GetDefaultAuraIntensity() const;
	float GetDefaultAuraScale() const;
};
