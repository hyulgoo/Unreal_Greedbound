// Fill out your copyright notice in the Description page of Project Settings.

#include "GBMovementStateComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Define/GBDefine.h"

EGBStanceState UGBMovementStateComponent::GetCharacterStanceState() const
{
    return StanceStateType;
}

void UGBMovementStateComponent::SetCharacterStanceState(const EGBStanceState NewState)
{
    StanceStateType = NewState;
}

EGBStopState UGBMovementStateComponent::GetCharacterStopState() const
{
    return StopStateType;
}

void UGBMovementStateComponent::SetCharacterStopState(const EGBStopState NewState)
{
    StopStateType = NewState;
}

EGBMoveState UGBMovementStateComponent::GetCharacterMoveState() const
{
    return MoveStateType;
}

void UGBMovementStateComponent::SetCharacterMoveState(const EGBMoveState NewState)
{
    MoveStateType = NewState;
}

UAnimMontage* UGBMovementStateComponent::GetTurnInPlaceMontage()
{
    return TurnInPlaceMontage;
}

void UGBMovementStateComponent::SetLastRotation()
{
    if (IsValid(GetOwner()))
    {
        LastRotation = GetOwner()->GetActorRotation();
    }
}

FRotator UGBMovementStateComponent::GetDeltaRotation() const
{
    GB_VALID_CHECK_WITH_RETURN(GetOwner(), FRotator::ZeroRotator);

    return UKismetMathLibrary::NormalizedDeltaRotator(GetOwner()->GetActorRotation(), LastRotation);
}

bool UGBMovementStateComponent::IsMovable() const
{
    return bIsMovable;
}

void UGBMovementStateComponent::SetMovable(const bool bMovable)
{
    bIsMovable = bMovable;
}
