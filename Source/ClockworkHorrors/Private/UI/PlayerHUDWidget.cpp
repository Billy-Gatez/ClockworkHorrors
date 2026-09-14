// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#include "UI/PlayerHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Components/ManaComponent.h"
#include "Utils/StatusEffectType.h"
#include "UI/StatusEffectIconWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeMana();
}

void UPlayerHUDWidget::NativeDestruct()
{
	if (ManaComponentRef)
	{
		ManaComponentRef->OnManaChanged.RemoveDynamic(this, &UPlayerHUDWidget::UpdateMana);
	}

	Super::NativeDestruct();
}

void UPlayerHUDWidget::InitializeMana()
{
	AActor* PlayerActor = GetOwningPlayerPawn();

	if (!PlayerActor)
	{
		PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}

	if (!PlayerActor)
	{
		return;
	}

	ManaComponentRef = PlayerActor->FindComponentByClass<UManaComponent>();

	if (!ManaComponentRef)
	{
		return;
	}

	ManaComponentRef->OnManaChanged.RemoveDynamic(this, &UPlayerHUDWidget::UpdateMana);
	ManaComponentRef->OnManaChanged.AddDynamic(this, &UPlayerHUDWidget::UpdateMana);

	UpdateMana(
		ManaComponentRef->GetCurrentMana(),
		ManaComponentRef->GetMaxMana()
	);
}

void UPlayerHUDWidget::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	HealthBar->SetPercent(CurrentHealth / MaxHealth);
	HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), FMath::RoundToInt(CurrentHealth), FMath::RoundToInt(MaxHealth))));
}

void UPlayerHUDWidget::UpdateXP(float CurrentXP, float MaxXP, int Level)
{
	XPBar->SetPercent(CurrentXP / MaxXP);
	XPText->SetText(FText::FromString(FString::Printf(TEXT("Level %d:  %d / %d"), Level, FMath::RoundToInt(CurrentXP), FMath::RoundToInt(MaxXP))));
}

void UPlayerHUDWidget::UpdateAmmo(int CurrentAmmo, int MaxAmmo, bool ShowOnHud)
{
	if (ShowOnHud)
	{
		AmmoText->SetVisibility(ESlateVisibility::Visible);
		AmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo)));
	}
	else
	{
		AmmoText->SetVisibility(ESlateVisibility::Hidden);
	}
	
}

void UPlayerHUDWidget::UpdateMana(float CurrentMana, float MaxMana)
{
	if (ManaBar)
	{
		ManaBar->SetPercent(MaxMana > 0.0f ? CurrentMana / MaxMana : 0.0f);
	}

	if (ManaText)
	{
		ManaText->SetText(
			FText::FromString(
				FString::Printf(
					TEXT("%d / %d"),
					FMath::RoundToInt(CurrentMana),
					FMath::RoundToInt(MaxMana)
				)
			)
		);
	}
}

void UPlayerHUDWidget::AddStatusEffectIcon(FName EffectType, float Duration)
{
	if (!StatusEffectIconWidgetClass || !StatusEffectIconsBox)
	{
		return;
	}

	RemoveStatusEffectIcon(EffectType);

	UStatusEffectIconWidget* IconWidget = CreateWidget<UStatusEffectIconWidget>(this, StatusEffectIconWidgetClass);
	if (!IconWidget)
	{
		return;
	}

	if (UTexture2D* const* FoundIcon = StatusEffectIcons.Find(EffectType))
	{
		IconWidget->SetIconImage(*FoundIcon, Duration);
	}

	StatusEffectIconsBox->AddChildToWrapBox(IconWidget);
	ActiveStatusEffectIconWidgets.Add(EffectType, IconWidget);
}

void UPlayerHUDWidget::RemoveStatusEffectIcon(FName EffectType)
{
	if (UStatusEffectIconWidget** FoundWidget = ActiveStatusEffectIconWidgets.Find(EffectType))
	{
		if (*FoundWidget)
		{
			(*FoundWidget)->RemoveFromParent();
		}
		ActiveStatusEffectIconWidgets.Remove(EffectType);
	}
}

void UPlayerHUDWidget::UpdateStatusEffects(float DeltaTime)
{
	for (auto It = ActiveStatusEffectIconWidgets.CreateIterator(); It; ++It)
	{
		UStatusEffectIconWidget* IconWidget = It.Value();
		if (IconWidget)
		{
			const float NewDuration = IconWidget->RemainingDuration - DeltaTime;
			if (NewDuration <= 0.0f)
			{
				It.RemoveCurrent();
				IconWidget->RemoveFromParent();
			}
			else
			{
				IconWidget->SetRemainingDuration(NewDuration);
			}
		}
	}
}
