// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.


#include "UI/StatusEffectIconWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UStatusEffectIconWidget::SetIconImage(UTexture2D* NewIcon, float Duration)
{
	if (IconImage && NewIcon)
	{
		IconImage->SetBrushFromTexture(NewIcon);
	}
	SetRemainingDuration(Duration);
}

void UStatusEffectIconWidget::SetRemainingDuration(float NewDuration)
{
	RemainingDuration = NewDuration;

	if (StatusEffectDurationText)
	{
		StatusEffectDurationText->SetText(FText::AsNumber(FMath::CeilToInt(RemainingDuration)));
	}
}
