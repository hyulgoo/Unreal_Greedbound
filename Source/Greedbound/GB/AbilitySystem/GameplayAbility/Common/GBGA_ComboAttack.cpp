// Fill out your copyright notice in the Description page of Project Settings.

#include "GBGA_ComboAttack.h"
#include "Define/GBAnimSectionName.h"
#include "Interface/GBCharacterCombatInterface.h"
#include "AbilitySystem/AbilityTask/GBAT_PlayMontageAndWaitForEvent.h"
#include "AbilitySystem/Attribute/GBCombatAttributeSet.h"
#include "Define/GBDefine.h"
#include "Define/GBTags.h"

UGBGA_ComboAttack::UGBGA_ComboAttack(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    EventTags.AddTag(GBTag::Event_Common_ComboCheck);
}

void UGBGA_ComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    GB_CONDITION_CHECK_WITHOUT_LOG(GetAvatarActorFromActorInfo()->GetLocalRole() >= ROLE_AutonomousProxy);

    TScriptInterface<IGBCharacterCombatInterface> CombatInterface = ActorInfo->AvatarActor.Get();
    if (CombatInterface)
    {
        CombatInterface->SetCombatState(EGBCombatState::Attack);

        UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
        GB_NULL_CHECK(ASC);

        GB_NULL_CHECK(ComboAttackMontage);
        GB_CONDITION_CHECK(EventTags.IsEmpty() == false);

        if (bShouldRotateNextComboStart)
        {
            RotateCharacterToControllerRotation();
        }

        PlayMontageAndWaitEventTask = UGBAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
            this,
            "PlayMontageAndWaitEvent",
            ComboAttackMontage,
            EventTags,
            1.f,
            GetNextSectionNameByIndex());
        PlayMontageAndWaitEventTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompleted);
        PlayMontageAndWaitEventTask->EventReceived.AddDynamic(this, &ThisClass::OnEventReceived);
        PlayMontageAndWaitEventTask->ReadyForActivation();
    }
}

void UGBGA_ComboAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    TScriptInterface<IGBCharacterCombatInterface> CombatInterface = ActorInfo->AvatarActor.Get();
    if (CombatInterface)
    {
        CombatInterface->SetCombatState(EGBCombatState::Idle);
    }

    CurrentComboIndex = 0;
    bShoudTriggerNextCombo = false;
}

FName UGBGA_ComboAttack::GetNextSectionNameByIndex()
{
    ++CurrentComboIndex;
    return FGBAnimSectionNameHelper::GetAttackSectionNameByComboIndex(CurrentComboIndex);
}

void UGBGA_ComboAttack::SetShouldNextCombo(const bool bTrigger)
{
    bShoudTriggerNextCombo = bTrigger;
}

void UGBGA_ComboAttack::RotateCharacterToControllerRotation()
{
    APawn* AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
    GB_NULL_CHECK(AvatarPawn);

    AController* AvatarController = AvatarPawn->GetController();
    GB_NULL_CHECK(AvatarController);

    const FRotator& ControlRot = AvatarController->GetControlRotation();
    AvatarPawn->SetActorRotation(FRotator(0.f, ControlRot.Yaw, 0.f));
}

void UGBGA_ComboAttack::OnCompleted(const FGameplayTag EventTag, FGameplayEventData EventData)
{
    K2_EndAbility();
}

void UGBGA_ComboAttack::OnEventReceived(const FGameplayTag EventTag, FGameplayEventData EventData)
{
    if (EventTag == GBTag::Event_Common_ComboCheck)
    {
        if (bShoudTriggerNextCombo)
        {
            bShoudTriggerNextCombo = false;
            if (bShouldRotateNextComboStart)
            {
                RotateCharacterToControllerRotation();
            }
            PlayMontageAndWaitEventTask->JumpToSection(GetNextSectionNameByIndex());
        }
    }
}

void UGBGA_ComboAttack::OnTargetDataReady(const FGameplayAbilityTargetDataHandle& DataHandle)
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    GB_NULL_CHECK(ASC);

    const UGBCombatAttributeSet* CombatAttributeSet = ASC->GetSet<UGBCombatAttributeSet>();
    GB_NULL_CHECK_WITHOUT_LOG(CombatAttributeSet);

    const float AttackPoint = CombatAttributeSet->GetAttack();

    for (TSharedPtr<FGameplayAbilityTargetData> TargetData : DataHandle.Data)
    {
        FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());
        GB_CONDITION_CHECK_WITHOUT_LOG(SpecHandle.IsValid());

        if (TargetData->HasHitResult())
        {
            const float HitDirection = GetHitDirectionToFloat(*TargetData->GetHitResult());
            SpecHandle.Data->SetSetByCallerMagnitude(GBTag::Data_Direction_Hit, HitDirection);
            SpecHandle.Data->SetSetByCallerMagnitude(GBTag::Data_Damage_Impact, -AttackPoint);
            TargetData->ApplyGameplayEffectSpec(*SpecHandle.Data.Get());
        }
    }
}

float UGBGA_ComboAttack::GetHitDirectionToFloat(const FHitResult& Hit)
{
    GB_CONDITION_CHECK_WITH_RETURN(Hit.bBlockingHit, false);

    AActor* Target = Hit.GetActor();
    GB_NULL_CHECK_WITH_RETURN(Target, false);

    const FVector& ActorLocation = Target->GetActorLocation();
    const FVector& ToHit = (Hit.ImpactPoint - ActorLocation).GetSafeNormal();

    const FVector& ActorForward = Target->GetActorForwardVector();
    const FVector& ActorRight = Target->GetActorRightVector();

    const float ForwardDot = FVector::DotProduct(ActorForward, ToHit);
    const float RightDot = FVector::DotProduct(ActorRight, ToHit);

    EGBDirection HitDirection;
    if (FMath::Abs(ForwardDot) > FMath::Abs(RightDot))
    {
        HitDirection = (ForwardDot >= 0) ? EGBDirection::Forward : EGBDirection::Backward;
    }
    else
    {
        HitDirection = (RightDot >= 0) ? EGBDirection::Right : EGBDirection::Left;
    }

    return static_cast<float>(HitDirection);
}
