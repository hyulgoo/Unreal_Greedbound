
#include "GBGameplayAbilityHelper.h"
#include "Abilities/GameplayAbility.h"
#include "Components/GBAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagsManager.h"
#include "Define/GBDefine.h"
#include "Data/GBInputData.h"
#include "GameplayAbility/Player/GBGA_PlayerSkill.h"

void FGBGameplayAbilityHelper::SendGameplayEventToTarget(const FGameplayTag EventTag, const FGameplayAbilityActorInfo* SourceActorInfo, const FGameplayAbilityActorInfo* TargetActorInfo, const float Magnitude)
{
    FGameplayEventData EventData;
    EventData.EventTag = EventTag;
    EventData.Instigator = SourceActorInfo->AvatarActor.Get();
    EventData.Target = TargetActorInfo->AvatarActor.Get();
    EventData.EventMagnitude = Magnitude;

    UAbilitySystemComponent* TargetASC = TargetActorInfo->AbilitySystemComponent.Get();
    if (TargetASC)
    {
        TargetASC->HandleGameplayEvent(EventData.EventTag, &EventData);
    }
    else
    {
        GB_LOG(TEXT("TargetASC is nullptr"));
    }
}

void FGBGameplayAbilityHelper::SendGameplayEventToTarget(const FGameplayTag EventTag, AActor* Instigator, UAbilitySystemComponent* TargetASC, const float Magnitude /*= 0.f*/)
{
    FGameplayEventData EventData;
    EventData.EventTag = EventTag;
    EventData.Instigator = Instigator;
    EventData.Target = TargetASC->GetOwnerActor();
    EventData.EventMagnitude = Magnitude;

    if (TargetASC)
    {
        TargetASC->HandleGameplayEvent(EventData.EventTag, &EventData);
    }
    else
    {
        GB_LOG(TEXT("TargetASC is nullptr"));
    }
}

void FGBGameplayAbilityHelper::SendGameplayEventToSelf(const FGameplayTag EventTag, const FGameplayAbilityActorInfo* SourceActorInfo, const float Magnitude /*= 0.f*/)
{
    UAbilitySystemComponent* SourceASC = SourceActorInfo->AbilitySystemComponent.Get();
    if (SourceASC)
    {
        SendGameplayEventToSelf(EventTag, SourceASC, Magnitude);
    }
    else
    {
        GB_LOG(TEXT("SourceASC is nullptr"));
    }
}

void FGBGameplayAbilityHelper::SendGameplayEventToSelf(const FGameplayTag EventTag, UAbilitySystemComponent* SourceASC, const float Magnitude /*= 0.f*/)
{
    if (SourceASC)
    {
        AActor* OwningActor = SourceASC->GetAvatarActor();
        SendGameplayEventToTarget(EventTag, OwningActor, SourceASC, Magnitude);
    }
    else
    {
        GB_LOG(TEXT("SourceASC is nullptr"));
    }
}

void FGBGameplayAbilityHelper::SendGameplayEventToSelf(const FGameplayTag EventTag, AActor* TargetActor, FGameplayEventData Payload)
{
    GB_CONDITION_CHECK(EventTag.IsValid());
    GB_VALID_CHECK(TargetActor);

    UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
    if (TargetASC)
    {
        TargetASC->HandleGameplayEvent(EventTag, &Payload);
    }
}

bool FGBGameplayAbilityHelper::HasMatchingTagGameplayTag(const FGameplayTag TagToCheck, UAbilitySystemComponent* TargetASC)
{
    GB_VALID_CHECK_WITH_RETURN(TargetASC, false);

    return TargetASC->HasMatchingGameplayTag(TagToCheck);
}

bool FGBGameplayAbilityHelper::HasMatchingTagGameplayTag(const FGameplayTag TagToCheck, AActor* TargetActor)
{
    GB_VALID_CHECK_WITH_RETURN(TargetActor, false);

    UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
    return HasMatchingTagGameplayTag(TagToCheck, TargetASC);
}

EGBTriggerType FGBGameplayAbilityHelper::GetAbilityTriggerTypeByTag(UAbilitySystemComponent* ASC, const FGameplayTag AssetTag)
{
    UGBAbilitySystemComponent* GBASC = Cast<UGBAbilitySystemComponent>(ASC);
    GB_NULL_CHECK_WITH_RETURN(GBASC, EGBTriggerType::None);

    return GBASC->GetAbilityTriggerTypeByTag(AssetTag);
}

UGBAbilitySystemComponent* FGBGameplayAbilityHelper::GetAbilitySystemComponentFromActor(AActor* Actor)
{
    UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
    if (ASC)
    {
        UGBAbilitySystemComponent* GBASC = Cast<UGBAbilitySystemComponent>(ASC);
        if (GBASC)
        {
            return GBASC;
        }

        GB_LOG(TEXT("Actor ASC is not GBASC"));
    }

    return nullptr;
}

void FGBGameplayAbilityHelper::GetActiveAbilityInstanceByClass(UAbilitySystemComponent* ASC, TSubclassOf<UGameplayAbility> AbilityClass, TArray<UGameplayAbility*>& OutList)
{
    OutList.Reset();

    if (ASC)
    {
        TArray<FGameplayAbilitySpec> AbilitySpecs = ASC->GetActivatableAbilities();
        for (const FGameplayAbilitySpec& Spec : AbilitySpecs)
        {
            if (Spec.Ability && Spec.Ability->GetClass()->IsChildOf(AbilityClass))
            {
                OutList = Spec.GetAbilityInstances();
            }
        }
    }
    else
    {
        GB_LOG(TEXT("TargetASC is nullptr"));
    }
}

void FGBGameplayAbilityHelper::GetAbilitySpecsByAllMatchingTags(UAbilitySystemComponent* ASC, const FGameplayTagContainer AssetTags, TArray<FGameplayAbilitySpec*>& Outlist)
{
    GB_NULL_CHECK(ASC);
    Outlist.Reset();

    ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(AssetTags, Outlist);
}

void FGBGameplayAbilityHelper::GetAbilitySpecsByTag(UAbilitySystemComponent* ASC, const FGameplayTag AssetTags, TArray<FGameplayAbilitySpec*>& Outlist)
{
    GB_NULL_CHECK(ASC);
    Outlist.Reset();

    UGBAbilitySystemComponent* GBASC = Cast<UGBAbilitySystemComponent>(ASC);
    GB_NULL_CHECK(GBASC);

    GBASC->GetAbilitySpecsByTag(AssetTags, Outlist);
}

FGameplayTagContainer FGBGameplayAbilityHelper::GetAllChildTag(const FGameplayTag ParentTag)
{
    return UGameplayTagsManager::Get().RequestGameplayTagChildren(ParentTag);
}

void FGBGameplayAbilityHelper::GetGameplayEffectDurationByTag(AActor* Target, const FGameplayTag EffectTag, float& OutRemainingTime, float& OutTotalDuration)
{
    UGBAbilitySystemComponent* GBASC = GetAbilitySystemComponentFromActor(Target);
    GB_NULL_CHECK(GBASC);

    GBASC->GetGameplayEffectDurationByTag(EffectTag, OutRemainingTime, OutTotalDuration);
}

void FGBGameplayAbilityHelper::GetGameplayEffectDurationByTag(UAbilitySystemComponent* ASC, const FGameplayTag EffectTag, float& OutRemainingTime, float& OutTotalDuration)
{
    UGBAbilitySystemComponent* GBASC = Cast<UGBAbilitySystemComponent>(ASC);
    GB_NULL_CHECK(GBASC);

    GBASC->GetGameplayEffectDurationByTag(EffectTag, OutRemainingTime, OutTotalDuration);
}

void FGBGameplayAbilityHelper::GetAbilityCoolTimeByTag(AActor* Target, const FGameplayTag AssetTag, float& OutRemainingTime, float& OutTotalCoolTime)
{
    UGBAbilitySystemComponent* GBASC = GetAbilitySystemComponentFromActor(Target);
    GB_NULL_CHECK(GBASC);

    GBASC->GetAbilityCoolTimeByTag(AssetTag, OutRemainingTime, OutTotalCoolTime);
}

void FGBGameplayAbilityHelper::GetAbilityCoolTimeByTag(UAbilitySystemComponent* ASC, const FGameplayTag AssetTag, float& OutRemainingTime, float& OutTotalCoolTime)
{
    UGBAbilitySystemComponent* GBASC = Cast<UGBAbilitySystemComponent>(ASC);
    GB_NULL_CHECK(GBASC);

    GBASC->GetAbilityCoolTimeByTag(AssetTag, OutRemainingTime, OutTotalCoolTime);
}

UTexture2D* FGBGameplayAbilityHelper::GetSkillAbilityIcon(UGameplayAbility* SkillAbility)
{
    GB_NULL_CHECK_WITH_RETURN(SkillAbility, nullptr);

    UGBGA_PlayerSkill* GBSkillAbility = Cast<UGBGA_PlayerSkill>(SkillAbility);
    GB_NULL_CHECK_WITH_RETURN(GBSkillAbility, nullptr);

    return GBSkillAbility->GetSkillIcon();
}
