// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.


#include "Components/ManaComponent.h"

// Sets default values for this component's properties
UManaComponent::UManaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UManaComponent::BeginPlay()
{
	Super::BeginPlay();

	MaxMana = FMath::Max(MaxMana, 0.0f);
	CurrentMana = MaxMana;
	ManaRegenDelayRemaining = 0.0f;
	ManaRecoveryBlockCount = 0;

	OnManaChanged.Broadcast(CurrentMana, MaxMana);
}

void UManaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bCanRegenerateMana || IsManaRecoveryBlocked() || CurrentMana >= MaxMana)
	{
		return;
	}

	if (ManaRegenDelayRemaining > 0.0f)
	{
		ManaRegenDelayRemaining = FMath::Max(ManaRegenDelayRemaining - DeltaTime, 0.0f);
		return;
	}

	RegenerateMana(ManaRegenRate * DeltaTime);
}

bool UManaComponent::ConsumeMana(float Amount)
{
	Amount = FMath::Max(Amount, 0.0f);

	if (!HasEnoughMana(Amount))
	{
		return false;
	}

	if (Amount <= 0.0f)
	{
		return true;
	}

	CurrentMana = FMath::Clamp(CurrentMana - Amount, 0.0f, MaxMana);
	ManaRegenDelayRemaining = FMath::Max(ManaRegenDelaySeconds, 0.0f);

	OnManaChanged.Broadcast(CurrentMana, MaxMana);

	return true;
}

void UManaComponent::RegenerateMana(float Amount)
{
	if (IsManaRecoveryBlocked())
	{
		return;
	}

	float OldMana = CurrentMana;
	CurrentMana = FMath::Clamp(CurrentMana + Amount, 0.0f, MaxMana);

	if (OldMana != CurrentMana)
	{
		OnManaChanged.Broadcast(CurrentMana, MaxMana);
	}
}

bool UManaComponent::HasEnoughMana(float Amount) const
{
	return CurrentMana >= FMath::Max(Amount, 0.0f);
}

float UManaComponent::GetManaPercentage() const
{
	return MaxMana > 0.0f ? CurrentMana / MaxMana : 0.0f;
}


float UManaComponent::GetCurrentMana() const
{
	return CurrentMana;
}

float UManaComponent::GetMaxMana() const
{
	return MaxMana;
}

void UManaComponent::SetCurrentMana(float NewCurrentMana)
{
	const float OldMana = CurrentMana;

	CurrentMana = FMath::Clamp(NewCurrentMana, 0.0f, MaxMana);

	if (OldMana != CurrentMana)
	{
		OnManaChanged.Broadcast(CurrentMana, MaxMana);
	}
}

void UManaComponent::SetMaxMana(float NewMaxMana, bool bFillMana)
{
	MaxMana = FMath::Max(NewMaxMana, 0.0f);

	if (bFillMana)
	{
		CurrentMana = MaxMana;
	}
	else
	{
		CurrentMana = FMath::Clamp(CurrentMana, 0.0f, MaxMana);
	}

	OnManaChanged.Broadcast(CurrentMana, MaxMana);
}

void UManaComponent::AddManaRecoveryBlock()
{
	ManaRecoveryBlockCount++;
}

void UManaComponent::RemoveManaRecoveryBlock()
{
	ManaRecoveryBlockCount = FMath::Max(ManaRecoveryBlockCount - 1, 0);
}

bool UManaComponent::IsManaRecoveryBlocked() const
{
	return ManaRecoveryBlockCount > 0;
}
