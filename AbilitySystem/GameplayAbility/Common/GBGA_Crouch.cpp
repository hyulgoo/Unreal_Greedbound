#include "GBGA_Crouch.h"
#include "Interface/GBCharacterMovementInterface.h"

UGBGA_Crouch::UGBGA_Crouch(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGBGA_Crouch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    TScriptInterface<IGBCharacterMovementInterface> CharacterMovementInterface = ActorInfo->AvatarActor.Get();
    if (CharacterMovementInterface)
    {
        CharacterMovementInterface->SetCharacterStanceState(EGBStanceState::Crouch);
    }
}

void UGBGA_Crouch::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    const bool bReplicatedEndAbility = true;
    const bool bWasCancelled = true;
    EndAbility(Handle, ActorInfo, ActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UGBGA_Crouch::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    TScriptInterface<IGBCharacterMovementInterface> CharacterMovementInterface = ActorInfo->AvatarActor.Get();
    if (CharacterMovementInterface)
    {
        CharacterMovementInterface->SetCharacterStanceState(EGBStanceState::Stand);
    }

    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

