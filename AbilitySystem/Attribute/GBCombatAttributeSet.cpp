// Fill out your copyright notice in the Description page of Project Settings.

#include "GBCombatAttributeSet.h"
#include "Net/UnrealNetwork.h"

bool UGBCombatAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
    return true;
}

void UGBCombatAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UGBCombatAttributeSet, Attack);
    DOREPLIFETIME(UGBCombatAttributeSet, Defense);
}
