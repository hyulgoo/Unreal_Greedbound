#include "BTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Interface/GBCharacterCombatInterface.h"
#include "Define/GBDefine.h"

UBTService_CheckAttackRange::UBTService_CheckAttackRange()
{
    NodeName = TEXT("Check Attack Range");
    Interval = 0.3f;

    TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_CheckAttackRange, TargetKey), AActor::StaticClass());
    IsInAttackRangeKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_CheckAttackRange, IsInAttackRangeKey));
}

void UBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
    GB_NULL_CHECK_WITHOUT_LOG(Pawn);

    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    AActor* Target = Cast<AActor>(BBComp->GetValueAsObject(TargetKey.SelectedKeyName));
    GB_NULL_CHECK_WITHOUT_LOG(Target);

    IGBCharacterCombatInterface* CombatInterface = Cast<IGBCharacterCombatInterface>(Pawn);
    GB_NULL_CHECK_WITHOUT_LOG(CombatInterface);

    // CombatData 삭제로 임시로 하드코딩
    const float AttackRange = 150.f;
    const float    DistSqr = FVector::DistSquared(Pawn->GetActorLocation(), Target->GetActorLocation());
    const bool bInRange = DistSqr <= FMath::Square(AttackRange);

    BBComp->SetValueAsBool(IsInAttackRangeKey.SelectedKeyName, bInRange);
}
