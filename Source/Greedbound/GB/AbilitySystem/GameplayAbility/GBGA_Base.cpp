// Fill out your copyright notice in the Description page of Project Settings.

#include "GBGA_Base.h"
#include "AbilitySystemComponent.h"
#include "Define/GBDefine.h"
#include "Define/GBTags.h"
#include "AbilitySystem/GBGameplayAbilityHelper.h"
#include "Data/GBInputData.h"

UGBGA_Base::UGBGA_Base(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    TargetBlockedTags.AddTag(GBTag::Block_All);
}

const FGameplayTagContainer& UGBGA_Base::GetActivationOwnedTags()
{
    return ActivationOwnedTags;
}

void UGBGA_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    GB_NULL_CHECK(ASC);

    if (FGBGameplayAbilityHelper::GetAbilityTriggerTypeByTag(ASC, GetTriggerTagByTag()) == EGBTriggerType::Toggle)
    {
        const FGameplayTagContainer& AssetTags = GetAssetTags();
        if (ASC->HasAllMatchingGameplayTags(AssetTags) == false)
        {
            ASC->AddLooseGameplayTags(AssetTags);
        }
    }
}

void UGBGA_Base::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    GB_NULL_CHECK(ASC);

    if (FGBGameplayAbilityHelper::GetAbilityTriggerTypeByTag(ASC, GetTriggerTagByTag()) == EGBTriggerType::Toggle)
    {
        const FGameplayTagContainer& AssetTags = GetAssetTags();
        if (IsPredictingClient())
        {
            if (ASC->HasAllMatchingGameplayTags(AssetTags))
            {
                ASC->RemoveLooseGameplayTags(AssetTags);
                ASC->ServerSetInputReleased(Handle);
            }
        }
        else if (HasAuthority(&ActivationInfo))
        {
            if (ASC->HasAllMatchingGameplayTags(AssetTags))
            {
                ASC->RemoveLooseGameplayTags(AssetTags);
            }
        }
    }
}

void UGBGA_Base::RemoveActiveEffectsByGrantedTag(const FGameplayTagContainer& Tags)
{
    GB_CONDITION_CHECK(Tags.IsValid());

    if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo_Ensured())
    {
        AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(Tags);
    }
}

void UGBGA_Base::RemoveActiveEffectsByGrantedTag(const FGameplayTag& Tag)
{
    RemoveActiveEffectsByGrantedTag(FGameplayTagContainer(Tag));
}

FGameplayTag UGBGA_Base::GetTriggerTagByTag() const
{
    const FGameplayTagContainer& AssetTags = GetAssetTags();
    return AssetTags.IsEmpty() ? FGameplayTag::EmptyTag :AssetTags.GetByIndex(0);
}
