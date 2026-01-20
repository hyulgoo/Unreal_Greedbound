#include "GBGA_Dead.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Interface/GBCharacterCombatInterface.h"
#include "Define/GBAnimSectionName.h"
#include "Define/GBDefine.h"

UGBGA_Dead::UGBGA_Dead(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGBGA_Dead::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    const FName DeathSectionName = FGBAnimSectionNameHelper::GetDeathSectionNameByDirection(TriggerEventData->EventMagnitude);

    GB_NULL_CHECK(DeadMontage);

    UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
        this,
        "PlayMontageAndWaitEvent",
        DeadMontage,
        1.f,
        DeathSectionName);
    PlayMontageTask->OnInterrupted.AddDynamic(this, &UGBGA_Dead::OnDeadSectionEnd);
    PlayMontageTask->OnCancelled.AddDynamic(this, &UGBGA_Dead::OnDeadSectionEnd);
    PlayMontageTask->OnCompleted.AddDynamic(this, &UGBGA_Dead::OnDeadSectionEnd);
    PlayMontageTask->ReadyForActivation();
}

void UGBGA_Dead::OnDeadSectionEnd()
{
    TScriptInterface<IGBCharacterCombatInterface> CombatInterface = CurrentActorInfo->AvatarActor.Get();
    if (CombatInterface && HasAuthority(&CurrentActivationInfo))
    {
        CombatInterface->BeRagdoll();
    }
}
