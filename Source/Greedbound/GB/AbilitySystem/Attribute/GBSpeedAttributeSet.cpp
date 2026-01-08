// Fill out your copyright notice in the Description page of Project Settings.

#include "GBSpeedAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "Define/GBDefine.h"
#include "Define/GBTags.h"
#include "AbilitySystem/GBGameplayAbilityHelper.h"

bool UGBSpeedAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
    return true;
}

void UGBSpeedAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    if (Attribute == GetCurrnetStaminaAttribute())
    {
        NewValue = NewValue <= 0.f ? 0.f : NewValue;
    }
}

void UGBSpeedAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
    GB_CONDITION_CHECK_WITHOUT_LOG(OldValue > 0.f);

    if (Attribute == GetCurrnetStaminaAttribute())
    {
        if (NewValue <= 0.f)
        {
            UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
            if (ASC && ASC->GetOwnerRole() == ENetRole::ROLE_Authority)
            {
                FGBGameplayAbilityHelper::SendGameplayEventToSelf(GBTag::Event_Common_StaminaEmpty, ASC);
            }
        }
    }
}

void UGBSpeedAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UGBSpeedAttributeSet, WalkSpeed);
    DOREPLIFETIME(UGBSpeedAttributeSet, SprintSpeed);
    DOREPLIFETIME(UGBSpeedAttributeSet, StaminaCost);
    DOREPLIFETIME(UGBSpeedAttributeSet, SprintStaminaThreshould);
    DOREPLIFETIME(UGBSpeedAttributeSet, CrouchSpeedCoefficient);

    DOREPLIFETIME_CONDITION_NOTIFY(UGBSpeedAttributeSet, CurrnetStamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UGBSpeedAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

void UGBSpeedAttributeSet::OnRep_CurrnetStamina(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UGBSpeedAttributeSet, CurrnetStamina, OldValue);
}

void UGBSpeedAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UGBSpeedAttributeSet, MaxStamina, OldValue);
}
