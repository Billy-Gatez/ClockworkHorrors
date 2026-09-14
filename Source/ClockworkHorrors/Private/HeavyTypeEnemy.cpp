// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#include "HeavyTypeEnemy.h"

#include "EnemyVisuals/EnemyAuraComponent.h"

#include "BaseCharacter.h"
#include "Engine/DamageEvents.h"


AHeavyTypeEnemy::AHeavyTypeEnemy()
{
	// =========================================================
	// DEFAULT STATS
	// =========================================================

	MoveSpeed = 150.0f;

	DetectionRadius = 750.0f;

	AttackRange = 175.0f;

	AttackDamage = 0.75f;

	AttackCooldown = 2.5f;


	// Heavy-specific defaults
	KnockbackForce = 900.0f;

	KnockbackUpForce = 250.0f;


	// ---------------------------------------------------------
	// ENEMY TYPE VISUAL
	// ---------------------------------------------------------

	if (EnemyAuraComponent)
	{
		EnemyAuraComponent->SetAuraType(
			EEnemyAuraType::Heavy
		);
	}
}


// =========================================================
// PERFORM ATTACK
// =========================================================

void AHeavyTypeEnemy::PerformAttack()
{
	Super::PerformAttack();

	OnHeavyAttack();
}

UStatusEffectType* AHeavyTypeEnemy::GetStatusEffectPayload(AActor* Target) const
{
	UStatusEffectType* StatusEffect = NewObject<UStatusEffectType>();

	StatusEffect->Effect = STATUSEFFECT::KnockBack;
	StatusEffect->Duration = KnockbackUpForce;
	StatusEffect->TickDamage = KnockbackForce;
	StatusEffect->TickInterval = 0;
	StatusEffect->Percentage = 0.0f; 

	StatusEffect->KnockbackDirection = GetActorForwardVector();

	return StatusEffect;
}
