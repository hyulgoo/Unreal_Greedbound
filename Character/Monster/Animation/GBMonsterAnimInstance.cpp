// Fill out your copyright notice in the Description page of Project Settings.

#include "GBMonsterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interface/GBCharacterMovementInterface.h"
#include "Define/GBDefine.h"

void UGBMonsterAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    FTimerDelegate BaseSpeedDelegate;
    BaseSpeedDelegate.BindUObject(this, &UGBMonsterAnimInstance::InitBaseSpeed);
    GetWorld()->GetTimerManager().SetTimer(SetBaseSpeedTimer, BaseSpeedDelegate, 1.f, false);
}

void UGBMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    GB_VALID_CHECK_WITHOUT_LOG(Movement);

    TScriptInterface<IGBCharacterMovementInterface> CharacterMovementInterface = Owner;
    GB_NULL_CHECK(CharacterMovementInterface);

    StanceStateType = CharacterMovementInterface->GetCharacterStanceState();
    StopStateType = CharacterMovementInterface->GetCharacterStopState();
    MoveStateType = CharacterMovementInterface->GetCharacterMoveState();

    Velocity = Movement->Velocity;
    GroundSpeed = Velocity.Size2D();
    bIsFalling = Movement->IsFalling();

    FVector NormalizedVelocity = Movement->Velocity.GetSafeNormal();
    Forward = NormalizedVelocity.Dot(Owner->GetActorForwardVector());
    Strafe = NormalizedVelocity.Dot(Owner->GetActorRightVector());
}

void UGBMonsterAnimInstance::InitBaseSpeed()
{
    Owner = Cast<ACharacter>(TryGetPawnOwner());
    if (Owner)
    {
        Movement = Owner->GetCharacterMovement();

        TScriptInterface<IGBCharacterMovementInterface>  CharacterMovementInterface = Owner;
        if (CharacterMovementInterface)
        {
            BaseWalkSpeed = CharacterMovementInterface->GetBaseWalkSpeed();
            BaseSprintSpeed = CharacterMovementInterface->GetBaseSprintSpeed();
        }
    }

    if (BaseWalkSpeed <= 1.f || BaseSprintSpeed <= 1.f)
    {
        FTimerDelegate BaseSpeedDelegate;
        BaseSpeedDelegate.BindUObject(this, &UGBMonsterAnimInstance::InitBaseSpeed);
        GetWorld()->GetTimerManager().SetTimer(SetBaseSpeedTimer, BaseSpeedDelegate, 1.f, false);
    }
}
