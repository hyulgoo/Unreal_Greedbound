#include "GBGA_RegenStamina.h"
#include "Interface/GBCharacterMovementInterface.h"
#include "Abilities/Tasks/AbilityTask_WaitAttributeChangeThreshold.h"
#include "AbilitySystem/Attribute/GBSpeedAttributeSet.h"
#include "Define/GBDefine.h"
#include "Define/GBTags.h"

UGBGA_RegenStamina::UGBGA_RegenStamina(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGBGA_RegenStamina::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    GB_CONDITION_CHECK_WITHOUT_LOG(CommitAbility(Handle, ActorInfo, ActivationInfo));

    TScriptInterface<IGBCharacterMovementInterface> CharacterMovementInterface = ActorInfo->AvatarActor.Get();
    if (CharacterMovementInterface)
    {
        UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
        GB_NULL_CHECK(ASC);

        const float MaxStamina = ASC->GetNumericAttribute(UGBSpeedAttributeSet::GetMaxStaminaAttribute());
        UAbilityTask_WaitAttributeChangeThreshold* ThresholdTask = UAbilityTask_WaitAttributeChangeThreshold::WaitForAttributeChangeThreshold(
            this,
            UGBSpeedAttributeSet::GetCurrnetStaminaAttribute(),
            EWaitAttributeChangeComparison::GreaterThanOrEqualTo,
            MaxStamina,
            true
        );
        GB_NULL_CHECK(ThresholdTask);

        ThresholdTask->OnChange.AddDynamic(this, &UGBGA_RegenStamina::OnCurrentStaminaChanged);
        ThresholdTask->ReadyForActivation();
    }
}

void UGBGA_RegenStamina::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    const bool bReplicatedEndAbility = true;
    const bool bWasCancelled = true;
    EndAbility(Handle, ActorInfo, ActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UGBGA_RegenStamina::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    TScriptInterface<IGBCharacterMovementInterface> CharacterMovementInterface = ActorInfo->AvatarActor.Get();
    if (CharacterMovementInterface)
    {
        FGameplayTagContainer GrantedTags(GBTag::Duration_Regen_Stamina);
        RemoveActiveEffectsByGrantedTag(GrantedTags);
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGBGA_RegenStamina::OnCurrentStaminaChanged(bool bMatchesComparison, float CurrentValue)
{
    if (bMatchesComparison)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
    }
}


