// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.


#include "MechEnemy.h"

void AMechEnemy::LandFromJump()
{
	UE_LOG(LogTemp, Warning, TEXT("MechEnemy has landed from jump!"));
	if (ShockWave)
	{
		FVector Loc = GetActorLocation();
		Loc.Z -= 150;
		FRotator Rot = GetActorRotation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = this;
		GetWorld()->SpawnActor<AActor>(ShockWave, Loc, Rot, SpawnParams);
	}
}

UStatusEffectType* AMechEnemy::GetStatusEffectPayload(AActor* Target) const
{
	return nullptr;
}
