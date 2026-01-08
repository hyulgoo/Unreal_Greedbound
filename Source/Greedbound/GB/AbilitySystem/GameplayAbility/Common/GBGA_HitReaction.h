// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/GBGA_Base.h"
#include "GBGA_HitReaction.generated.h"

UCLASS()
class GREEDBOUND_API UGBGA_HitReaction : public UGBGA_Base
{
    GENERATED_BODY()

public:
    UGBGA_HitReaction(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
                                
    virtual void                ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override final;
    virtual void                EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override final;

private:
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UAnimMontage>    HitReactionMontage;
};
