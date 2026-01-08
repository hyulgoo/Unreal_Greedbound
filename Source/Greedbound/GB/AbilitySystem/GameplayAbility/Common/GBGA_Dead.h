// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/GBGA_Base.h"
#include "GBGA_Dead.generated.h"

UCLASS()
class GREEDBOUND_API UGBGA_Dead : public UGBGA_Base
{
    GENERATED_BODY()
    
public:
    UGBGA_Dead(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void                ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override final;

private:
    UFUNCTION()
    void                        OnDeadSectionEnd();

private:
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UAnimMontage>    DeadMontage;
};
