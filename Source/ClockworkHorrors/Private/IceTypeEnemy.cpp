// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#include "IceTypeEnemy.h"

#include "EnemyVisuals/EnemyAuraComponent.h"

#include "BaseCharacter.h"
#include "Engine/DamageEvents.h"


AIceTypeEnemy::AIceTypeEnemy()
{
	// =========================================================
	// DEFAULT STATS
	// =========================================================

	MoveSpeed = 225.0f;

	DetectionRadius = 850.0f;

	AttackRange = 450.0f;

	AttackDamage = 0.20f;

	AttackCooldown = 1.25f;


	// Ice-specific defaults
	SlowMultiplier = 0.50f;

	SlowDuration = 2.5f;


	// ---------------------------------------------------------
	// ENEMY TYPE VISUAL
	// ---------------------------------------------------------

	if (EnemyAuraComponent)
	{
		EnemyAuraComponent->SetAuraType(
			EEnemyAuraType::Ice
		);
	}
}


// =========================================================
// PERFORM ATTACK
// =========================================================

void AIceTypeEnemy::PerformAttack()
{
	Super::PerformAttack();

	OnIceAttack();
}

UStatusEffectType* AIceTypeEnemy::GetStatusEffectPayload(AActor* Target) const
{
	UStatusEffectType* StatusEffect = NewObject<UStatusEffectType>();

	StatusEffect->Effect = STATUSEFFECT::Slowed;
	StatusEffect->Duration = SlowDuration;
	StatusEffect->TickDamage = 0.0f;
	StatusEffect->TickInterval = 0.0f;
	StatusEffect->Percentage = SlowMultiplier;

	return StatusEffect;
}
