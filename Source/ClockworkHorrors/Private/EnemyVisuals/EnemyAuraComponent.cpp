#include "EnemyVisuals/EnemyAuraComponent.h"

#if WITH_EDITOR
#include "UObject/UnrealType.h"
#endif

UEnemyAuraComponent::UEnemyAuraComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetAutoActivate(true);
}

void UEnemyAuraComponent::BeginPlay()
{
	Super::BeginPlay();
	ApplyAuraSettings();
}

void UEnemyAuraComponent::OnRegister()
{
	Super::OnRegister();
	ApplyAuraSettings();
}

#if WITH_EDITOR
void UEnemyAuraComponent::PostEditChangeProperty(
	FPropertyChangedEvent& PropertyChangedEvent
)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	ApplyAuraSettings();
}
#endif

void UEnemyAuraComponent::SetAuraType(EEnemyAuraType NewAuraType)
{
	if (AuraType == NewAuraType)
	{
		RefreshAura();
		return;
	}

	AuraType = NewAuraType;
	ApplyAuraSettings();
}

void UEnemyAuraComponent::SetAuraEnabled(bool bEnabled)
{
	if (bAuraEnabled == bEnabled)
	{
		RefreshAura();
		return;
	}

	bAuraEnabled = bEnabled;
	ApplyAuraSettings();
}

void UEnemyAuraComponent::RefreshAura()
{
	ApplyAuraSettings();
}

void UEnemyAuraComponent::ApplyAuraSettings()
{
	if (!ShouldAuraBeVisible())
	{
		Deactivate();
		SetVisibility(false, true);
		return;
	}

	SetVisibility(true, true);

	SetVariableLinearColor(
		AuraColorParameterName,
		GetResolvedAuraColor()
	);

	SetVariableFloat(
		AuraIntensityParameterName,
		GetResolvedAuraIntensity()
	);

	SetVariableFloat(
		AuraScaleParameterName,
		GetResolvedAuraScale()
	);

	if (GetAsset())
	{
		Activate(true);
	}
}

bool UEnemyAuraComponent::ShouldAuraBeVisible() const
{
	if (!bAuraEnabled)
	{
		return false;
	}

	if (AuraType == EEnemyAuraType::None)
	{
		return false;
	}

	if (
		AuraType == EEnemyAuraType::Normal &&
		!bShowNormalAura
	)
	{
		return false;
	}

	return true;
}

FLinearColor UEnemyAuraComponent::GetResolvedAuraColor() const
{
	if (bOverrideAuraColor)
	{
		return AuraColorOverride;
	}

	return GetDefaultAuraColor();
}

float UEnemyAuraComponent::GetResolvedAuraIntensity() const
{
	if (bOverrideAuraIntensity)
	{
		return FMath::Max(0.0f, AuraIntensityOverride);
	}

	return GetDefaultAuraIntensity();
}

float UEnemyAuraComponent::GetResolvedAuraScale() const
{
	if (bOverrideAuraScale)
	{
		return FMath::Max(0.0f, AuraScaleOverride);
	}

	return GetDefaultAuraScale();
}

FLinearColor UEnemyAuraComponent::GetDefaultAuraColor() const
{
	switch (AuraType)
	{
	case EEnemyAuraType::Normal:
		return FLinearColor(0.45f, 0.45f, 0.45f, 1.0f);

	case EEnemyAuraType::Fire:
		return FLinearColor(1.0f, 0.06f, 0.0f, 1.0f);

	case EEnemyAuraType::Ice:
		return FLinearColor(0.0f, 0.55f, 1.0f, 1.0f);

	case EEnemyAuraType::Poison:
		return FLinearColor(0.12f, 1.0f, 0.03f, 1.0f);

	case EEnemyAuraType::Heavy:
		return FLinearColor(1.0f, 0.38f, 0.02f, 1.0f);

	case EEnemyAuraType::None:
	default:
		return FLinearColor::White;
	}
}

float UEnemyAuraComponent::GetDefaultAuraIntensity() const
{
	switch (AuraType)
	{
	case EEnemyAuraType::Normal:
		return 0.35f;

	case EEnemyAuraType::Heavy:
		return 1.35f;

	case EEnemyAuraType::Fire:
	case EEnemyAuraType::Ice:
	case EEnemyAuraType::Poison:
		return 1.0f;

	case EEnemyAuraType::None:
	default:
		return 0.0f;
	}
}

float UEnemyAuraComponent::GetDefaultAuraScale() const
{
	switch (AuraType)
	{
	case EEnemyAuraType::Normal:
		return 0.8f;

	case EEnemyAuraType::Heavy:
		return 1.15f;

	case EEnemyAuraType::Fire:
	case EEnemyAuraType::Ice:
	case EEnemyAuraType::Poison:
		return 1.0f;

	case EEnemyAuraType::None:
	default:
		return 1.0f;
	}
}
