// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "GBAbilitySystemComponent.generated.h"

class UGBInputData;
class UGBAttributeData;
enum class EGBTriggerType : uint8;

UCLASS()
class GREEDBOUND_API UGBAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:
    UGBAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    void                            AbilityTagTriggered(FGameplayTag AssetTag);
    void                            AbilityTagReleased(FGameplayTag AssetTag);
    void                            AbilityTagToggled(FGameplayTag AssetTag);
    void                            SetInputData(UGBInputData* NewInputData);

    const FGameplayTagContainer&    GetActivationOwnedTags(UGameplayAbility* Ability) const;
    EGBTriggerType                  GetAbilityTriggerTypeByTag(FGameplayTag AssetTag) const;
    void                            GetAbilitySpecsByTag(FGameplayTag AssetTag, TArray<FGameplayAbilitySpec*>& Outlist);
    void                            GetGameplayEffectDurationByTag(FGameplayTag EffectTag, float& OutRemainingTime, float& OutTotalDuration);
    void                            GetAbilityCoolTimeByTag(FGameplayTag AssetTag, float& OutRemainingTime, float& OutTotalDuration);

    void                            InitAttributeStats(UGBAttributeData* AttributeInfo);
    
private:
    UPROPERTY()
    TSubclassOf<UGameplayEffect>    InitAttributeStatGameplayEffectClass;

    UPROPERTY()
    TObjectPtr<UGBInputData>        InputData;
};
