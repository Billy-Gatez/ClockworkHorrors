// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.


#include "UI/ButtonWithText.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
void UButtonWithText::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (Information) {
		Information->SetText(Info);
		Information->SetColorAndOpacity(TextColor);
	}

	if (BackgroundButton) {
		BackgroundButton->SetBackgroundColor(ButtonColor);
		FSlateBrush brush;
		if (texture)
		{
		brush.SetResourceObject(texture);
		brush.DrawAs = ESlateBrushDrawType::Image;
		FButtonStyle style = BackgroundButton->GetStyle();
		style.Normal = brush;
		style.Hovered = brush;
		style.Pressed = brush;
		BackgroundButton->SetStyle(style);
		}
	}
}

void UButtonWithText::NativeConstruct()
{
	Super::NativeConstruct();
	if (BackgroundButton) {
		BackgroundButton->OnClicked.AddDynamic(this, &UButtonWithText::HandleButtonClicked);
	}
}

void UButtonWithText::HandleButtonClicked()
{
	InternalButtonClicked.Broadcast();
	InternalButtonClickedWithSource.Broadcast(this);
}