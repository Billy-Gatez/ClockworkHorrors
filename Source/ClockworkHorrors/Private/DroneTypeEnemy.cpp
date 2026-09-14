// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.


#include "DroneTypeEnemy.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/AIC_CodeEnemyController.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

ADroneTypeEnemy::ADroneTypeEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	TrueCollision = CreateDefaultSubobject<USphereComponent>(TEXT("TrueCollision"));
	TrueCollision->SetupAttachment(RootComponent);
	DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMesh"));
	DroneMesh->SetupAttachment(TrueCollision);
	FVector CurrentLocation = GetCapsuleComponent()->GetRelativeLocation();
	CurrentLocation.Z = HoverHeight; // Set the Z position to the desired hover height
	TrueCollision->SetRelativeLocation(CurrentLocation);
	
	MoveSpeed = 300.0f;

	DroneBehaviorTree = nullptr;
}

void ADroneTypeEnemy::BeginPlay()
{
	Super::BeginPlay();

	if( UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->SetMovementMode(MOVE_Flying);
		MoveComp->MaxFlySpeed = MoveSpeed;
		MoveComp->BrakingFrictionFactor = 2.0f; // Adjust braking friction for smoother stopping
	}

	if(DroneBehaviorTree)
	{
		if (AAIC_CodeEnemyController* AIController = Cast<AAIC_CodeEnemyController>(GetController()))
		{
			AIController->SetBehaviorTree(DroneBehaviorTree);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("DroneTypeEnemy: Controller is not of type AAIC_CodeEnemyController."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DroneTypeEnemy: DroneBehaviorTree is not set."));
	}
}

void ADroneTypeEnemy::Tick(float DeltaTime)
{
	FVector CurrentLocation = GetCapsuleComponent()->GetRelativeLocation();
	CurrentLocation.Z = HoverHeight; // Set the Z position to the desired hover height
	TrueCollision->SetRelativeLocation(CurrentLocation);
}
