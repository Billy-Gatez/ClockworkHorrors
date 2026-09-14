// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_JumpTowardsPlayer.generated.h"

struct FBTJumpTowardsPlayerMemory
{
	TWeakObjectPtr<ACharacter> JumpingCharacter;
	TWeakObjectPtr<UBehaviorTreeComponent> OwnerBTComp;
	float ElapsedTime = 0.f;
};

/**
 * 
 */
UCLASS()
class CLOCKWORKHORRORS_API UBTTask_JumpTowardsPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_JumpTowardsPlayer();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 SenderID, bool bSuccess) override;

	virtual uint16 GetInstanceMemorySize() const override;

	UFUNCTION()
	void HandleLanded(const FHitResult& Hit);

	UFUNCTION()
	void OnCharacterLanded(AAIController* AIController, ACharacter* Character);

protected:
	/** Height (0-1) of the jump arc, where 0 is a flat trajectory and 1 is a very high arc. */
	UPROPERTY(EditAnywhere, Category = "Jump")
	float JumpArcHeight = 0.5f;

private:
	TWeakObjectPtr<ACharacter> BoundCharacter;
};
