#pragma once

#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"

class UGBGA_Base;
class UAbilitySystemComponent;
class UGBAbilitySystemComponent;
enum class EGBTriggerType : uint8;

class FGBGameplayAbilityHelper
{
public:
    static UGBAbilitySystemComponent*   GetAbilitySystemComponentFromActor(AActor* Actor);
    static void                         GetActiveAbilityInstanceByClass(UAbilitySystemComponent* ASC, TSubclassOf<UGameplayAbility> AbilityClass, TArray<UGameplayAbility*>& OutList);

    static void                         SendGameplayEventToTarget(const FGameplayTag EventTag, const FGameplayAbilityActorInfo* SourceActorInfo, const FGameplayAbilityActorInfo* TargetActorInfo, const float Magnitude = 0.f);
    static void                         SendGameplayEventToTarget(const FGameplayTag EventTag, AActor* Instigator, UAbilitySystemComponent* TargetASC, const float Magnitude = 0.f);
    static void                         SendGameplayEventToSelf(const FGameplayTag EventTag, const FGameplayAbilityActorInfo* SourceActorInfo, const float Magnitude = 0.f);
    static void                         SendGameplayEventToSelf(const FGameplayTag EventTag, UAbilitySystemComponent* SourceASC, const float Magnitude = 0.f);
    static void                         SendGameplayEventToSelf(const FGameplayTag EventTag, AActor* TargetActor, FGameplayEventData Payload);

    static bool                         HasMatchingTagGameplayTag(const FGameplayTag TagToCheck, UAbilitySystemComponent* TargetASC);
    static bool                         HasMatchingTagGameplayTag(const FGameplayTag TagToCheck, AActor* TargetActor);

    static EGBTriggerType               GetAbilityTriggerTypeByTag(UAbilitySystemComponent* ASC, const FGameplayTag AssetTag);

    static void                         GetAbilitySpecsByAllMatchingTags(UAbilitySystemComponent* ASC, const FGameplayTagContainer AssetTags, TArray<FGameplayAbilitySpec*>& Outlist);
    static void                         GetAbilitySpecsByTag(UAbilitySystemComponent* ASC, const FGameplayTag AssetTags, TArray<FGameplayAbilitySpec*>& Outlist);

    static FGameplayTagContainer        GetAllChildTag(const FGameplayTag ParentTag);

    static void                         GetGameplayEffectDurationByTag(AActor* Target, const FGameplayTag EffectTag, float& OutRemainingTime, float& OutTotalDuration);
    static void                         GetGameplayEffectDurationByTag(UAbilitySystemComponent* ASC, const FGameplayTag EffectTag, float& OutRemainingTime, float& OutTotalDuration);
    static void                         GetAbilityCoolTimeByTag(AActor* Target, const FGameplayTag InputTag, float& OutRemainingTime, float& OutTotalCoolTime);
    static void                         GetAbilityCoolTimeByTag(UAbilitySystemComponent* ASC, const FGameplayTag InputTag, float& OutRemainingTime, float& OutTotalCoolTime);

    static UTexture2D*                  GetSkillAbilityIcon(UGameplayAbility* SkillAbility);
};

