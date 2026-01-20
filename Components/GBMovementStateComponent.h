// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/GBCharacterStateTypes.h"
#include "GBMovementStateComponent.generated.h"

class UAnimMontage;

namespace GBMovementConstants
{
    constexpr float MoveThreshold                   = 1.f;
    constexpr float TurnThresholdForAutonomous      = 60.f;
    constexpr float TurnThresholdForSimulatedProxy  = 10.f;
}

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GREEDBOUND_API UGBMovementStateComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    EGBStanceState              GetCharacterStanceState() const;
    void                        SetCharacterStanceState(const EGBStanceState NewState);
    EGBStopState                GetCharacterStopState() const;
    void                        SetCharacterStopState(const EGBStopState NewState);
    EGBMoveState                GetCharacterMoveState() const;
    void                        SetCharacterMoveState(const EGBMoveState NewState);
    UAnimMontage*               GetTurnInPlaceMontage();
    void                        SetLastRotation();
    FRotator                    GetDeltaRotation() const;
    bool                        IsMovable() const;
    void                        SetMovable(const bool bMovable);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
    TObjectPtr<UAnimMontage>    TurnInPlaceMontage;

private:
    FRotator                    LastRotation;
    EGBStanceState              StanceStateType     = EGBStanceState::Stand;
    EGBStopState                StopStateType       = EGBStopState::Stop;
    EGBMoveState                MoveStateType       = EGBMoveState::Walk;
    uint8                       bIsMovable : 1      = true;
};
