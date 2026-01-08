// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/GBCharacterStateTypes.h"
#include "GBCombatComponent.generated.h"

class UGBCombatData;
class AGBWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GREEDBOUND_API UGBCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    void                    SetCombatState(const EGBCombatState NewState);
    EGBCombatState          GetCombatState() const;

    void                    EquipWeapon(AGBWeapon* NewWeapon);
    UStaticMeshComponent*   GetWeaponMesh() const;
    float                   GetWeaponAttack() const;

    void                    SetWeaponTrailEffect(const bool bActive);

private:
    EGBCombatState          CombatState = EGBCombatState::Idle;

    UPROPERTY()
    TObjectPtr<AGBWeapon>   Weapon;
};
