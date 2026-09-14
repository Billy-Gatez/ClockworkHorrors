#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CircleOfRejuvenation.generated.h"

UCLASS()
class CLOCKWORKHORRORS_API UBTTask_CircleOfRejuvenation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_CircleOfRejuvenation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActor;
};