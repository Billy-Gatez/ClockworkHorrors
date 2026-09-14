// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpellSelectorWidget.generated.h"

class UTextBlock;
class UVerticalBox;

UCLASS()
class CLOCKWORKHORRORS_API USpellSelectorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSpellNames(
		const FText& PreviousSpellName,
		const FText& CurrentSpellName,
		const FText& NextSpellName
	);

	void AnimateSpellChange(
		int32 Direction,
		const FText& PreviousSpellName,
		const FText& CurrentSpellName,
		const FText& NextSpellName
	);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> SpellList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PreviousSpellText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentSpellText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NextSpellText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell Selector|Animation", meta = (ClampMin = "0.01"))
	float SpinDuration = 0.16f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell Selector|Animation", meta = (ClampMin = "1.0"))
	float SpinDistance = 48.0f;

private:
	void ApplyPendingSpellNames();
	void ResetVisualState();

	FText PendingPreviousSpellName;
	FText PendingCurrentSpellName;
	FText PendingNextSpellName;

	bool bAnimating = false;
	float AnimationElapsed = 0.0f;
	int32 AnimationDirection = 0;
};
