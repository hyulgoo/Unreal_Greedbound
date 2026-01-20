// Fill out your copyright notice in the Description page of Project Settings.

#include "GBCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/GBAbilitySystemComponent.h"
#include "Components/GBMovementStateComponent.h"
#include "Components/GBCombatComponent.h"
#include "Define/GBDefine.h"
#include "Define/GBTags.h"
#include "Define/GBCollision.h"
#include "AbilitySystem/Attribute/GBSpeedAttributeSet.h"
#include "Subsystem/Asset/GBAssetLoadSubsystem.h"

AGBCharacterBase::AGBCharacterBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bReplicates = true;
    SetReplicatingMovement(true);

    CombatComponent = CreateDefaultSubobject<UGBCombatComponent>(TEXT("CombatComponent"));
    MovementStateComponent = CreateDefaultSubobject<UGBMovementStateComponent>(TEXT("MovementStateComponent"));
}

void AGBCharacterBase::BeginPlay()
{
    Super::BeginPlay();
}

void AGBCharacterBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AGBCharacterBase::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    // Bone 및 Socket 위치는 카메라 프러스텀 기준으로 갱신되는데 서버는 렌더링 기능이 없으므로 
    // 서버에서만 호출되는 PossessBy에서 해당 조건을 완화함
    GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesAndRefreshBonesWhenPlayingMontages;
}

UAbilitySystemComponent* AGBCharacterBase::GetAbilitySystemComponent() const
{
    return Cast<UAbilitySystemComponent>(AbilitySystemComponent);
}

void AGBCharacterBase::SetAbilitySystemComponent(UGBAbilitySystemComponent* ASC)
{
    GB_NULL_CHECK(ASC);
    AbilitySystemComponent = ASC;

    AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(GBTag::Event_Common_StaminaEmpty).AddUObject(this, &AGBCharacterBase::OnSprintEndEventRecieved);
    AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(GBTag::Event_Common_TrailAttackBegin).AddUObject(this, &AGBCharacterBase::OnAttackEventRecieved);
    AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(GBTag::Event_Common_TrailAttackEnd).AddUObject(this, &AGBCharacterBase::OnAttackEventRecieved);
    AbilitySystemComponent->RegisterGameplayTagEvent(GBTag::State_Movement_Sprint, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AGBCharacterBase::OnMovementStateTagChanged);
    AbilitySystemComponent->RegisterGameplayTagEvent(GBTag::State_Movement_Crouch, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AGBCharacterBase::OnMovementStateTagChanged);
    AbilitySystemComponent->RegisterGameplayTagEvent(GBTag::Block_All, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AGBCharacterBase::OnMovementStateTagChanged);
    AbilitySystemComponent->RegisterGameplayTagEvent(GBTag::Block_Input, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AGBCharacterBase::OnMovementStateTagChanged);
    AbilitySystemComponent->RegisterGameplayTagEvent(GBTag::Block_Move, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AGBCharacterBase::OnMovementStateTagChanged);
}

void AGBCharacterBase::Multicast_BeRagdoll_Implementation()
{
    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetCollisionProfileName(GBCOLLISION_PROFILE_RAGDOLL);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGBCharacterBase::InitAbilitySystemComponent(AActor* OwnerActor)
{
    GB_VALID_CHECK(OwnerActor);
    GB_NULL_CHECK(AbilitySystemComponent);

    if (HasAuthority())
    {
        LoadCharacterData();
    }

    AbilitySystemComponent->InitAbilityActorInfo(OwnerActor, this);
}

UGBMovementStateComponent* AGBCharacterBase::GetMovementStateComponent() const
{
    return MovementStateComponent;
}

bool AGBCharacterBase::IsPlayingMontage(UAnimMontage* Montage)
{
    GB_NULL_CHECK_WITH_RETURN(Montage, false);

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    return IsValid(AnimInstance) && AnimInstance->Montage_IsPlaying(Montage);
}

void AGBCharacterBase::SetCharacterStanceState(const EGBStanceState NewState)
{
    GetMovementStateComponent()->SetCharacterStanceState(NewState);
    UpdateSpeed();
}

void AGBCharacterBase::SetCharacterStopState(const EGBStopState NewState)
{
    GetMovementStateComponent()->SetCharacterStopState(NewState);
}

void AGBCharacterBase::SetCharacterMoveState(const EGBMoveState NewState)
{
    GetMovementStateComponent()->SetCharacterMoveState(NewState);
    UpdateSpeed();
}

EGBStanceState AGBCharacterBase::GetCharacterStanceState() const
{
    return GetMovementStateComponent()->GetCharacterStanceState();
}

EGBStopState AGBCharacterBase::GetCharacterStopState() const
{
    return GetMovementStateComponent()->GetCharacterStopState();
}

EGBMoveState AGBCharacterBase::GetCharacterMoveState() const
{
    return GetMovementStateComponent()->GetCharacterMoveState();
}

float AGBCharacterBase::GetCrouchSpeedCoefficient() const
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
    GB_NULL_CHECK_WITH_RETURN(ASC, 0.f);

    FGameplayAttribute CrouchSpeedCoefficientAttribute = UGBSpeedAttributeSet::GetCrouchSpeedCoefficientAttribute();
    return ASC->GetNumericAttribute(CrouchSpeedCoefficientAttribute);
}

float AGBCharacterBase::GetBaseWalkSpeed() const
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
    GB_NULL_CHECK_WITH_RETURN(ASC, 0.f);

    FGameplayAttribute WalkSpeedAttribute = UGBSpeedAttributeSet::GetWalkSpeedAttribute();
    return ASC->GetNumericAttribute(WalkSpeedAttribute);
}

float AGBCharacterBase::GetBaseSprintSpeed() const
{
    UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
    GB_NULL_CHECK_WITH_RETURN(ASC, 0.f);

    FGameplayAttribute SprintSpeedAttribute = UGBSpeedAttributeSet::GetSprintSpeedAttribute();
    return ASC->GetNumericAttribute(SprintSpeedAttribute);
}

void AGBCharacterBase::UpdateSpeed()
{
    const bool bIsSprint = GetCharacterMoveState() == EGBMoveState::Sprint;
    const FGameplayAttribute SpeedAttribute = bIsSprint ? UGBSpeedAttributeSet::GetSprintSpeedAttribute() : UGBSpeedAttributeSet::GetWalkSpeedAttribute();
    UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
    GB_NULL_CHECK_WITHOUT_LOG(ASC);

    float NewSpeed = ASC->GetNumericAttribute(SpeedAttribute);
    if (GetCharacterStanceState() == EGBStanceState::Crouch)
    {
        NewSpeed *= GetCrouchSpeedCoefficient();
    }

    GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void AGBCharacterBase::SetCombatState(const EGBCombatState NewState)
{
    CombatComponent->SetCombatState(NewState);
}

EGBCombatState AGBCharacterBase::GetCombatState() const
{
    return CombatComponent->GetCombatState();
}

void AGBCharacterBase::BeRagdoll()
{
    if (HasAuthority())
    {
        Multicast_BeRagdoll();
    }
}

void AGBCharacterBase::EquipWeapon(AGBWeapon* NewWeapon)
{
    GB_NULL_CHECK(NewWeapon);
    GB_NULL_CHECK(AbilitySystemComponent);

    if (HasAuthority())
    {
        CombatComponent->EquipWeapon(NewWeapon);
        AbilitySystemComponent->AddLooseGameplayTag(GBTag::State_EquipState_Equipped);
        AbilitySystemComponent->AddReplicatedLooseGameplayTag(GBTag::State_EquipState_Equipped);
    }
}

UStaticMeshComponent* AGBCharacterBase::GetWeaponMesh() const
{
    return CombatComponent->GetWeaponMesh();
}

void AGBCharacterBase::OnSprintEndEventRecieved(const FGameplayEventData* Payload)
{
    GB_NULL_CHECK(Payload);
    GB_CONDITION_CHECK(Payload->EventTag == GBTag::Event_Common_StaminaEmpty);

    if (HasAuthority())
    {
        UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
        if (ASC)
        {
            FGameplayTagContainer CancelTag(GBTag::Trigger_Common_Sprint);
            ASC->CancelAbilities(&CancelTag);
        }
    }
}

void AGBCharacterBase::OnAttackEventRecieved(const FGameplayEventData* Payload)
{
    GB_CONDITION_CHECK_WITHOUT_LOG(HasAuthority());

    GB_NULL_CHECK(Payload);
    GB_NULL_CHECK(AbilitySystemComponent);

    if (AbilitySystemComponent->HasMatchingGameplayTag(GBTag::State_EquipState_Equipped))
    {
        if (Payload->EventTag == GBTag::Event_Common_TrailAttackBegin)
        {
            CombatComponent->SetWeaponTrailEffect(true);
        }
        else if (Payload->EventTag == GBTag::Event_Common_TrailAttackEnd)
        {
            CombatComponent->SetWeaponTrailEffect(false);
        }
    }
}

void AGBCharacterBase::OnMovementStateTagChanged(const FGameplayTag Tag, const int32 Count)
{
    if (Tag == GBTag::State_Movement_Crouch)
    {
        MovementStateComponent->SetCharacterStanceState(Count > 0 ? EGBStanceState::Crouch : EGBStanceState::Stand);
    }
    else if (Tag == GBTag::State_Movement_Sprint)
    {
        MovementStateComponent->SetCharacterMoveState(Count > 0 ? EGBMoveState::Sprint : EGBMoveState::Walk);
    }
    else if (Tag == GBTag::Block_All || Tag == GBTag::Block_Input || Tag == GBTag::Block_Move)
    {
        FGameplayTagContainer MoveBlockTags(GBTag::Block_All);
        MoveBlockTags.AddTag(GBTag::Block_Input);
        MoveBlockTags.AddTag(GBTag::Block_Move);
        MovementStateComponent->SetMovable(GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(MoveBlockTags) == false);
    }
}

void AGBCharacterBase::LoadCharacterData()
{
    GB_CONDITION_CHECK(CharacterDataAssetId.IsValid());

    UGBAssetLoadSubsystem::Get(this)->LoadCharacterDataByAssetId(CharacterDataAssetId, AbilitySystemComponent);
}
