// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#include "FireTypeEnemy.h"

#include "EnemyVisuals/EnemyAuraComponent.h"

#include "BaseCharacter.h"
#include "Engine/DamageEvents.h"


AFireTypeEnemy::AFireTypeEnemy()
{
	// ---------------------------------------------------------
	// FIRE ENEMY DEFAULT STATS
	// ---------------------------------------------------------

	MoveSpeed = 275.0f;

	DetectionRadius = 900.0f;

	AttackRange = 400.0f;

	AttackDamage = 0.30f;

	AttackCooldown = 1.0f;

	BurnDamage = 0.05f;

	BurnDuration = 3.0f;


	// ---------------------------------------------------------
	// ENEMY TYPE VISUAL
	// ---------------------------------------------------------

	if (EnemyAuraComponent)
	{
		EnemyAuraComponent->SetAuraType(
			EEnemyAuraType::Fire
		);
	}
}


// =========================================================
// FIRE ATTACK
// =========================================================

void AFireTypeEnemy::PerformAttack()
{
	Super::PerformAttack();


	// Blueprint handles visuals / audio.
	OnFireAttack();
}

UStatusEffectType* AFireTypeEnemy::GetStatusEffectPayload(AActor* Target) const
{
	UStatusEffectType* StatusEffect = NewObject<UStatusEffectType>();

	StatusEffect->Effect = STATUSEFFECT::Burning;
	StatusEffect->Duration = BurnDuration;
	StatusEffect->TickDamage = BurnDamage;
	StatusEffect->TickInterval = BurnTickInterval;
	StatusEffect->Percentage = 0.0f;

	return StatusEffect;
}
