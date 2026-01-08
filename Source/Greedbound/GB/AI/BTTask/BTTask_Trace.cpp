#include "BTTask_Trace.h"
#include "AIController.h"
#include "AbilitySystemInterface.h"
#include "Define/GBTags.h"
#include "Define/GBDefine.h"
#include "AbilitySystem/GBGameplayAbilityHelper.h"

EBTNodeResult::Type UBTTask_Trace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    GB_NULL_CHECK_WITH_RETURN(AIController, EBTNodeResult::Failed);

    APawn* AIPawn = AIController->GetPawn();
    GB_NULL_CHECK_WITH_RETURN(AIPawn, EBTNodeResult::Failed);

    IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(AIPawn);
    GB_NULL_CHECK_WITH_RETURN(ASCInterface, EBTNodeResult::Failed);

    UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent();
    GB_NULL_CHECK_WITH_RETURN(ASC, EBTNodeResult::Failed);

    FGBGameplayAbilityHelper::SendGameplayEventToSelf(GBTag::Trigger_Common_Sprint, ASC);

    return Super::ExecuteTask(OwnerComp, NodeMemory);
}
