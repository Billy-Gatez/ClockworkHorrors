// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#include "NormalTypeEnemy.h"

#include "EnemyVisuals/EnemyAuraComponent.h"


ANormalTypeEnemy::ANormalTypeEnemy()
{
	// =========================================================
	// DEFAULT NORMAL ENEMY STATS
	// =========================================================

	MoveSpeed = 250.0f;

	DetectionRadius = 800.0f;

	AttackRange = 150.0f;

	AttackDamage = 0.20f;

	AttackCooldown = 0.50f;


	// ---------------------------------------------------------
	// ENEMY TYPE VISUAL
	// ---------------------------------------------------------

	if (EnemyAuraComponent)
	{
		EnemyAuraComponent->SetAuraType(
			EEnemyAuraType::Normal
		);
	}
}

UStatusEffectType* ANormalTypeEnemy::GetStatusEffectPayload(AActor* Target) const
{
	return nullptr;
}
