// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#include "UI/EnemyHealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/ProgressBar.h"

void UEnemyHealthBarWidget::SetHealth(float CurrentHealth, float MaxHealth)
{
    if (!HealthBar)
    {
        return;
    }

    if (MaxHealth <= 0.0f)
    {
        HealthBar->SetPercent(0.0f);
        return;
    }

    const float HealthPercent = CurrentHealth / MaxHealth;

    HealthBar->SetPercent(HealthPercent);
}

