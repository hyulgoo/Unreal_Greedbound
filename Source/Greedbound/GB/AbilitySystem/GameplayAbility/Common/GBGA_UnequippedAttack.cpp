// Fill out your copyright notice in the Description page of Project Settings.

#include "GBGA_UnequippedAttack.h"
#include "Interface/GBCharacterCombatInterface.h"
#include "Abilities/GameplayAbilityTargetActor_Trace.h"
#include "Define/GBDefine.h"
#include "Define/GBTags.h"

UGBGA_UnequippedAttack::UGBGA_UnequippedAttack(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    EventTags.AddTag(GBTag::Event_Common_Attack);
}

void UGBGA_UnequippedAttack::OnEventReceived(const FGameplayTag EventTag, FGameplayEventData EventData)
{
    Super::OnEventReceived(EventTag, EventData);

    if (EventTag == GBTag::Event_Common_Attack)
    {
        AActor* AvatarActor = GetAvatarActorFromActorInfo();
        GB_NULL_CHECK_WITHOUT_LOG(AvatarActor);

        const FVector& Start = AvatarActor->GetActorLocation();
        const FVector& ForwardVector = AvatarActor->GetActorForwardVector();
        const FVector& End = Start + (ForwardVector * AttackRange);

        UWorld* World = GetWorld();

        FHitResult HitResult;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(AvatarActor);

        FName AttackProfile;
        if (TScriptInterface<IGBCharacterCombatInterface> CombatInterface = AvatarActor)
        {
            AttackProfile = CombatInterface->GetAttackProfile();
        }

        AGameplayAbilityTargetActor_Trace::LineTraceWithFilter(HitResult, World, FGameplayTargetDataFilterHandle(), Start, End, AttackProfile, Params);
        if (HitResult.bBlockingHit)
        {
            FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
            FGameplayAbilityTargetDataHandle TargetDataHandle;
            TargetDataHandle.Data.Add(TSharedPtr<FGameplayAbilityTargetData>(TargetData));

            OnTargetDataReady(TargetDataHandle);
        }
#if WITH_EDITOR
        DrawDebugLine(World, Start, End, HitResult.bBlockingHit ? FColor::Red : FColor::Green, false, 1.f, 0, 2.f);
#endif
    }
}
