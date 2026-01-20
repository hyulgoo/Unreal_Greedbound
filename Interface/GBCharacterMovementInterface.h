// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Character/GBCharacterStateTypes.h"
#include "GBCharacterMovementInterface.generated.h"

UINTERFACE(MinimalAPI)
class UGBCharacterMovementInterface : public UInterface
{
    GENERATED_BODY()
};

class GREEDBOUND_API IGBCharacterMovementInterface
{
    GENERATED_BODY()

public:
    virtual void                    SetCharacterStanceState(const EGBStanceState NewState) = 0;
    virtual void                    SetCharacterStopState(const EGBStopState NewState) = 0;
    virtual void                    SetCharacterMoveState(const EGBMoveState NewState) = 0;

    virtual EGBStanceState          GetCharacterStanceState() const = 0;
    virtual EGBStopState            GetCharacterStopState() const = 0;
    virtual EGBMoveState            GetCharacterMoveState() const = 0;

    virtual float                   GetCrouchSpeedCoefficient() const = 0;
    virtual float                   GetBaseWalkSpeed() const = 0;
    virtual float                   GetBaseSprintSpeed() const = 0;
};
