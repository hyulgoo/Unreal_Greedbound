#include "GBGA_HitReaction.h"
#include "Define/GBAnimSectionName.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/Attribute/GBHealthAttributeSet.h"
#include "Define/GBDefine.h"

UGBGA_HitReaction::UGBGA_HitReaction(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGBGA_HitReaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (HasAuthority(&CurrentActivationInfo) || IsPredictingClient())
    {
        GB_NULL_CHECK(HitReactionMontage);

        const FName HitReactionSectionName = FGBAnimSectionNameHelper::GetHitReactionNameByDirection(TriggerEventData->EventMagnitude);
        if (HitReactionSectionName != GBANIMSECTIONNAME_HITREACTION_NOREACT)
        {
            UAbilityTask_PlayMontageAndWait* MontagePlayTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "HitReaction", HitReactionMontage, 1.f, HitReactionSectionName);
            MontagePlayTask->ReadyForActivation();
        }
    }
}

void UGBGA_HitReaction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if(ASC)
    {
        ASC->ApplyModToAttribute(UGBHealthAttributeSet::GetHitDirectionAttribute(), EGameplayModOp::Override, 0.f);
    }
}
