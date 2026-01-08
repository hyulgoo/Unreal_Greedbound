// Fill out your copyright notice in the Description page of Project Settings.

#include "GBCombatComponent.h"
#include "InteractionObject/Weapon/GBWeapon.h"
#include "Define/GBDefine.h"

void UGBCombatComponent::SetCombatState(const EGBCombatState NewState)
{
    CombatState = NewState;
}

EGBCombatState UGBCombatComponent::GetCombatState() const
{
    return CombatState;
}

void UGBCombatComponent::EquipWeapon(AGBWeapon* NewWeapon)
{
    GB_NULL_CHECK(NewWeapon);

    if (GetOwner()->HasAuthority())
    {
        if (Weapon)
        {
            Weapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
            Weapon->SetOwner(nullptr);
            Weapon = nullptr;
        }

        AActor* OwningActor = GetOwner();
        GB_NULL_CHECK(OwningActor);

        Weapon = NewWeapon;
        Weapon->SetOwner(OwningActor);

        USkeletalMeshComponent* SkeletalMesh = OwningActor->GetComponentByClass<USkeletalMeshComponent>();
        GB_NULL_CHECK(SkeletalMesh);

        Weapon->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, Weapon->GetAttachSocketName());
    }
}

UStaticMeshComponent* UGBCombatComponent::GetWeaponMesh() const
{
    GB_VALID_CHECK_WITH_RETURN(Weapon, nullptr);

    return Weapon->GetComponentByClass<UStaticMeshComponent>();
}

float UGBCombatComponent::GetWeaponAttack() const
{
    GB_VALID_CHECK_WITH_RETURN(Weapon, 0.f);

    return Weapon->GetAttack();
}

void UGBCombatComponent::SetWeaponTrailEffect(const bool bActive)
{
    GB_VALID_CHECK(Weapon);

    Weapon->SetActiveTrailEffect(bActive);
}
