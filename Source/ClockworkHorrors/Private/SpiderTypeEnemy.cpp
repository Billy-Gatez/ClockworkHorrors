// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.


#include "SpiderTypeEnemy.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "AI/AIC_CodeEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utils/StatusEffectType.h"
#include "BaseCharacter.h"
#include "AI/BaseCompanion.h"
#include "Utils/HealthComponent.h"

ASpiderTypeEnemy::ASpiderTypeEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	TrueCollision = CreateDefaultSubobject<USphereComponent>(TEXT("TrueCollision"));
	TrueCollision->SetupAttachment(GetMesh());
}

void ASpiderTypeEnemy::PerformAttack()
{
	AttackTypeInt = FMath::RandRange(0, 2);
	Super::PerformAttack();	
}

UStatusEffectType* ASpiderTypeEnemy::GetStatusEffectPayload(AActor* Target) const
{
	UStatusEffectType* StatusEffect = NewObject<UStatusEffectType>();

	switch (AttackTypeInt)
	{
	case 0:
	{
		// Regular Attack
		break;
	}
	case 1:
	{
		// Implement Stun attack behavior here
		StatusEffect->Effect = STATUSEFFECT::Stunned;
		//StatusEffect->NextStatusEffect = To Apply Another Status Effect
		StatusEffect->Duration = stunTime;
		/*StatusEffect->TickDamage = BurnDamage;
		StatusEffect->TickInterval = BurnTickInterval;*/
		StatusEffect->Percentage = SlowPercentage; // Only used for slowed. 
		break;
	}
	case 2:
	{
		// Implement Poison attack behavior here
		StatusEffect->Effect = STATUSEFFECT::Poisoned;
		StatusEffect->Duration = PoisonDuration;
		StatusEffect->TickDamage = PoisonTickDamage;
		StatusEffect->TickInterval = PoisonTickInterval;
		//StatusEffect->Percentage = 0.0f;
		break;
	}
	default:
		break;
	}

	

	return StatusEffect;
}
