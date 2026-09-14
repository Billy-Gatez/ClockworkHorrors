#include "BTTask_CircleOfRejuvenation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "CompanionAbilityComponent.h"

UBTTask_CircleOfRejuvenation::UBTTask_CircleOfRejuvenation()
{
	NodeName = "Cast Circle Of Rejuvenation";
}

EBTNodeResult::Type UBTTask_CircleOfRejuvenation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ACharacter* CompanionCharacter = Cast<ACharacter>(AIController->GetPawn());
	if (!CompanionCharacter) return EBTNodeResult::Failed;

	UCompanionAbilityComponent* AbilityComp = CompanionCharacter->FindComponentByClass<UCompanionAbilityComponent>();
	if (!AbilityComp) return EBTNodeResult::Failed;

	// 1. GUARD: Do NOT cast if companion is resting or seated
	if (AbilityComp->GetIsResting() || AbilityComp->GetIsFullySeated())
	{
		return EBTNodeResult::Failed;
	}

	// 2. GUARD: Do NOT cast if companion doesn't have enough Essence
	if (AbilityComp->GetCurrentEssence() < AbilityComp->RejuvenationCost)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return EBTNodeResult::Failed;

	AActor* TargetPlayer = Cast<AActor>(BBComp->GetValueAsObject(TargetActor.SelectedKeyName));
	if (!TargetPlayer) return EBTNodeResult::Failed;

	// Delegate ability execution to component
	const bool bSuccess = AbilityComp->CastCircleOfRejuvenation(TargetPlayer);

	return bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}