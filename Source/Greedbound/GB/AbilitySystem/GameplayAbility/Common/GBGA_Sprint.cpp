#include "GBGA_Sprint.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attribute/GBSpeedAttributeSet.h"
#include "Interface/GBCharacterMovementInterface.h"
#include "Define/GBDefine.h"
#include "Define/GBTags.h"

constexpr float DelayToRegenStamina = 1.f;
constexpr float MimStamina = 10.f;

UGBGA_Sprint::UGBGA_Sprint(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGBGA_Sprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    GB_CONDITION_CHECK_WITH_RETURN_WITHOUT_LOG(Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags), false);

    FGameplayAttribute SpeedAttribute = UGBSpeedAttributeSet::GetCurrnetStaminaAttribute();
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if (ASC)
    {
        const float CurrnetStamina = ASC->GetNumericAttribute(SpeedAttribute);
        if (MimStamina > CurrnetStamina)
        {
            return false;
        }
    }

    return true;
}

void UGBGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    GB_CONDITION_CHECK_WITHOUT_LOG(CommitAbility(Handle, ActorInfo, ActivationInfo));

    TScriptInterface<IGBCharacterMovementInterface> CharacterMovementInterface = ActorInfo->AvatarActor.Get();
    if (CharacterMovementInterface)
    {
        CharacterMovementInterface->SetCharacterMoveState(EGBMoveState::Sprint);
    }

    if (RegenStaminaTimer.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(RegenStaminaTimer);
    }
}

void UGBGA_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    Super::InputReleased(Handle, ActorInfo, ActivationInfo);

    const bool bReplicatedEndAbility = true;
    const bool bWasCancelled = true;
    EndAbility(Handle, ActorInfo, ActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UGBGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    TScriptInterface<IGBCharacterMovementInterface> CharacterMovementInterface = ActorInfo->AvatarActor.Get();
    if (CharacterMovementInterface)
    {
        CharacterMovementInterface->SetCharacterMoveState(EGBMoveState::Walk);

        RemoveActiveEffectsByGrantedTag(GBTag::Duration_Cost_Stamina);

        // DelayToRegenStamina 이후 RegenStamina GA Event 발동
        FTimerDelegate LamdaDelegate = FTimerDelegate::CreateLambda([this]()
            {
                UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
                GB_NULL_CHECK_WITHOUT_LOG(ASC);

                AActor* TargetActor = GetActorInfo().AvatarActor.Get();
                FGameplayEventData EventData;
                EventData.EventTag = GBTag::Trigger_Common_RegenStamina;
                EventData.Target = TargetActor;
                EventData.TargetTags = ASC->GetOwnedGameplayTags();

                ASC->HandleGameplayEvent(EventData.EventTag, &EventData);
            });

        GetWorld()->GetTimerManager().SetTimer(RegenStaminaTimer, LamdaDelegate, DelayToRegenStamina, false);
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
