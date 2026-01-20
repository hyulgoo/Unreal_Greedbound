// Fill out your copyright notice in the Description page of Project Settings.

#include "GBGA_Jump.h"
#include "GameFramework/Character.h"

UGBGA_Jump::UGBGA_Jump(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGBGA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) == false)
    {
        return false;
    }

    const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    return (Character && Character->CanJump());
}

void UGBGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGBGA_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
    {
        Character->StopJumping();
    }
}

void UGBGA_Jump::OnLandedCallback()
{
    K2_EndAbility();
}
