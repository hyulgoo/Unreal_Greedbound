// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GB/AbilitySystem/GameplayAbility/GBGA_Base.h"
#include "GBGA_Crouch.generated.h"

UCLASS()
class GREEDBOUND_API UGBGA_Crouch : public UGBGA_Base
{
    GENERATED_BODY()

public:
    UGBGA_Crouch(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override final;
    virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override final;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override final;
};
