// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.


#include "AI/BTTask_JumpTowardsPlayer.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/EnemyInterface.h"

UBTTask_JumpTowardsPlayer::UBTTask_JumpTowardsPlayer()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_JumpTowardsPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	ACharacter* AIPawn = Cast<ACharacter>(AIController->GetPawn());
	if (!AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Player = Cast<APawn>(BlackboardComp->GetValueAsObject("Target"));

	if (!Player)
	{
		return EBTNodeResult::Failed;
	}

	FVector PlayerLocation = Player->GetActorLocation();
	FVector StartLocation = AIPawn->GetActorLocation();

	FVector LaunchVelocity;
	const bool bFoundArc = UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		AIPawn,
		LaunchVelocity,
		StartLocation,
		PlayerLocation,
		0.f,
		JumpArcHeight);

	if (!bFoundArc)
	{
		return EBTNodeResult::Failed;
	}

	AIPawn->LaunchCharacter(LaunchVelocity, true, true);

	AIPawn->LandedDelegate.AddDynamic(this, &UBTTask_JumpTowardsPlayer::HandleLanded);

	FBTJumpTowardsPlayerMemory* Memory = reinterpret_cast<FBTJumpTowardsPlayerMemory*>(NodeMemory);
	Memory->JumpingCharacter = AIPawn;
	Memory->OwnerBTComp = &OwnerComp;
	Memory->ElapsedTime = 0.f;

	BoundCharacter = AIPawn;
	AIPawn->LandedDelegate.AddDynamic(this, &UBTTask_JumpTowardsPlayer::HandleLanded);

	return EBTNodeResult::InProgress;
}

void UBTTask_JumpTowardsPlayer::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 SenderID, bool bSuccess)
{
	if (Message == "ActionFinished")
	{
		UE_LOG(LogTemp, Warning, TEXT("BTTask_JumpTowardsPlayer: Received ActionFinished message, finishing task"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

uint16 UBTTask_JumpTowardsPlayer::GetInstanceMemorySize() const
{
	return sizeof(FBTJumpTowardsPlayerMemory);
}

void UBTTask_JumpTowardsPlayer::HandleLanded(const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("BTTask_JumpTowardsPlayer: LandedDelegate fired"));

	ACharacter* Character = BoundCharacter.Get();
	UBehaviorTreeComponent* BTComp = nullptr;

	if (Character)
	{
		Character->LandedDelegate.RemoveDynamic(this, &UBTTask_JumpTowardsPlayer::HandleLanded);

		if (AAIController* AIController = Cast<AAIController>(Character->GetController()))
		{
			BTComp = Cast<UBehaviorTreeComponent>(AIController->BrainComponent);
			OnCharacterLanded(AIController, Character);
		}
	}

	BoundCharacter = nullptr;

	if (BTComp)
	{
		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_JumpTowardsPlayer::OnCharacterLanded(AAIController* AIController, ACharacter* Character)
{
	IEnemyInterface* Enemy = Cast<IEnemyInterface>(Character);
	if (Enemy)
	{
		Enemy->LandFromJump();
	}
}
