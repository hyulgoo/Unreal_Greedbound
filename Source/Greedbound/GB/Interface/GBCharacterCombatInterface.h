// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Character/GBCharacterStateTypes.h"
#include "GBCharacterCombatInterface.generated.h"

class UGBCombatData;
class AGBWeapon;

UINTERFACE(MinimalAPI)
class UGBCharacterCombatInterface : public UInterface
{
    GENERATED_BODY()
};

class GREEDBOUND_API IGBCharacterCombatInterface
{
    GENERATED_BODY()

public:
    virtual void                    SetCombatState(const EGBCombatState NewState) = 0;
    virtual EGBCombatState          GetCombatState() const = 0;
    virtual void                    BeRagdoll() = 0;
    virtual void                    EquipWeapon(AGBWeapon* NewWeapon) = 0;
    virtual UStaticMeshComponent*   GetWeaponMesh() const = 0;
    virtual FName                   GetAttackProfile() const = 0;
};
