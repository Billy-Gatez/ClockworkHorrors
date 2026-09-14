// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 *
 */

UCLASS()
class CLOCKWORKHORRORS_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* HealthText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* XPBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* XPText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* ManaBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ManaText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWrapBox* StatusEffectIconsBox;

	UPROPERTY(EditDefaultsOnly, Category = "Status Effect")
	TSubclassOf<class UStatusEffectIconWidget> StatusEffectIconWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Status Effect")
	TMap<FName, UTexture2D*> StatusEffectIcons;

	UPROPERTY(Transient)
	TMap<FName, class UStatusEffectIconWidget*> ActiveStatusEffectIconWidgets;

	UPROPERTY(Transient)
	class UManaComponent* ManaComponentRef;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* AmmoText;
	void InitializeMana();

public:
	UFUNCTION()
	void UpdateHealth(float CurrentHealth, float MaxHealth);

	UFUNCTION()
	void UpdateXP(float CurrentXP, float MaxXP, int Level);
	UFUNCTION()
	void UpdateAmmo(int CurrentAmmo, int MaxAmmo, bool ShowOnHud);

	UFUNCTION()
	void UpdateMana(float CurrentMana, float MaxMana);

	UFUNCTION(BlueprintCallable, Category = "Status Effect")
	void AddStatusEffectIcon(FName EffectType, float Duration);

	UFUNCTION(BlueprintCallable, Category = "Status Effect")
	void RemoveStatusEffectIcon(FName EffectName);

	UFUNCTION()
	void UpdateStatusEffects(float DeltaTime);

};
