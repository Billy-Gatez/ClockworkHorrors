// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#include "SpellSelectorWidget.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void USpellSelectorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResetVisualState();
}

void USpellSelectorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bAnimating || !SpellList)
	{
		return;
	}

	AnimationElapsed += InDeltaTime;

	const float Alpha = FMath::Clamp(
		AnimationElapsed / FMath::Max(SpinDuration, 0.01f),
		0.0f,
		1.0f
	);

	const float SmoothedAlpha = FMath::InterpEaseInOut(
		0.0f,
		1.0f,
		Alpha,
		2.0f
	);

	const float VerticalOffset =
		-AnimationDirection *
		SpinDistance *
		SmoothedAlpha;

	SpellList->SetRenderTranslation(FVector2D(0.0f, VerticalOffset));

	const float CurrentOpacity = FMath::Lerp(1.0f, 0.35f, SmoothedAlpha);
	const float CurrentScale = FMath::Lerp(1.0f, 0.85f, SmoothedAlpha);

	const float IncomingOpacity = FMath::Lerp(0.35f, 1.0f, SmoothedAlpha);
	const float IncomingScale = FMath::Lerp(0.85f, 1.0f, SmoothedAlpha);

	const float FarOpacity = FMath::Lerp(0.35f, 0.10f, SmoothedAlpha);
	const float FarScale = FMath::Lerp(0.85f, 0.75f, SmoothedAlpha);

	if (CurrentSpellText)
	{
		CurrentSpellText->SetRenderOpacity(CurrentOpacity);
		CurrentSpellText->SetRenderScale(FVector2D(CurrentScale, CurrentScale));
	}

	UTextBlock* IncomingSpellText =
		AnimationDirection > 0
		? NextSpellText.Get()
		: PreviousSpellText.Get();

	UTextBlock* FarSpellText =
		AnimationDirection > 0
		? PreviousSpellText.Get()
		: NextSpellText.Get();

	if (IncomingSpellText)
	{
		IncomingSpellText->SetRenderOpacity(IncomingOpacity);
		IncomingSpellText->SetRenderScale(FVector2D(IncomingScale, IncomingScale));
	}

	if (FarSpellText)
	{
		FarSpellText->SetRenderOpacity(FarOpacity);
		FarSpellText->SetRenderScale(FVector2D(FarScale, FarScale));
	}

	if (Alpha >= 1.0f)
	{
		ApplyPendingSpellNames();
		bAnimating = false;
		AnimationElapsed = 0.0f;
		AnimationDirection = 0;
		ResetVisualState();
	}
}

void USpellSelectorWidget::SetSpellNames(
	const FText& PreviousSpellName,
	const FText& CurrentSpellName,
	const FText& NextSpellName
)
{
	bAnimating = false;
	AnimationElapsed = 0.0f;
	AnimationDirection = 0;

	PendingPreviousSpellName = PreviousSpellName;
	PendingCurrentSpellName = CurrentSpellName;
	PendingNextSpellName = NextSpellName;

	ApplyPendingSpellNames();
	ResetVisualState();
}

void USpellSelectorWidget::AnimateSpellChange(
	int32 Direction,
	const FText& PreviousSpellName,
	const FText& CurrentSpellName,
	const FText& NextSpellName
)
{
	if (Direction == 0)
	{
		SetSpellNames(
			PreviousSpellName,
			CurrentSpellName,
			NextSpellName
		);

		return;
	}

	if (bAnimating)
	{
		ApplyPendingSpellNames();
		ResetVisualState();
	}

	PendingPreviousSpellName = PreviousSpellName;
	PendingCurrentSpellName = CurrentSpellName;
	PendingNextSpellName = NextSpellName;

	AnimationDirection = Direction > 0 ? 1 : -1;
	AnimationElapsed = 0.0f;
	bAnimating = true;
}

void USpellSelectorWidget::ApplyPendingSpellNames()
{
	if (PreviousSpellText)
	{
		PreviousSpellText->SetText(PendingPreviousSpellName);
	}

	if (CurrentSpellText)
	{
		CurrentSpellText->SetText(PendingCurrentSpellName);
	}

	if (NextSpellText)
	{
		NextSpellText->SetText(PendingNextSpellName);
	}
}

void USpellSelectorWidget::ResetVisualState()
{
	if (SpellList)
	{
		SpellList->SetRenderTranslation(FVector2D::ZeroVector);
	}

	if (PreviousSpellText)
	{
		PreviousSpellText->SetRenderOpacity(0.35f);
		PreviousSpellText->SetRenderScale(FVector2D(0.85f, 0.85f));
	}

	if (CurrentSpellText)
	{
		CurrentSpellText->SetRenderOpacity(1.0f);
		CurrentSpellText->SetRenderScale(FVector2D(1.0f, 1.0f));
	}

	if (NextSpellText)
	{
		NextSpellText->SetRenderOpacity(0.35f);
		NextSpellText->SetRenderScale(FVector2D(0.85f, 0.85f));
	}
}
