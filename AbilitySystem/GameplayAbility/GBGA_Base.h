// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GBGA_Base.generated.h"

UCLASS()
class GREEDBOUND_API UGBGA_Base : public UGameplayAbility
{
    GENERATED_BODY()
    
public:
    UGBGA_Base(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    const FGameplayTagContainer&    GetActivationOwnedTags();

    virtual void                    ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void                    InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
    void                            RemoveActiveEffectsByGrantedTag(const FGameplayTagContainer& Tags);
    void                            RemoveActiveEffectsByGrantedTag(const FGameplayTag& Tag);

private:
    FGameplayTag                    GetTriggerTagByTag() const;
};
