// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusEffectIconWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLOCKWORKHORRORS_API UStatusEffectIconWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* IconImage;
public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* StatusEffectDurationText;


	UPROPERTY(BlueprintReadOnly)
	float RemainingDuration = 0.0f;

	void SetIconImage(UTexture2D* NewIcon, float Duration);

	void SetRemainingDuration(float NewDuration);
	
};
