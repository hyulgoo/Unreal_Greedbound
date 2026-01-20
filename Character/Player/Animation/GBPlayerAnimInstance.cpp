
#include "GBPlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interface/GBCharacterMovementInterface.h"
#include "Define/GBDefine.h"

void UGBPlayerAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    FTimerDelegate BaseSpeedDelegate;
    BaseSpeedDelegate.BindUObject(this, &UGBPlayerAnimInstance::InitBaseSpeed);
    GetWorld()->GetTimerManager().SetTimer(SetBaseSpeedTimer, BaseSpeedDelegate, 1.f, false);
}

void UGBPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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
    bIsJumping = bIsFalling && (Velocity.Z > JumpingThreshould);

    FVector NormalizedVelocity = Movement->Velocity.GetSafeNormal();
    Forward = NormalizedVelocity.Dot(Owner->GetActorForwardVector());
    Strafe = NormalizedVelocity.Dot(Owner->GetActorRightVector());

    LastRotation = CurrentRotation;
    CurrentRotation = Owner->GetActorRotation();
    DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentRotation, LastRotation);
}

void UGBPlayerAnimInstance::InitBaseSpeed()
{
    Owner = Cast<ACharacter>(TryGetPawnOwner());
    if (Owner)
    {
        Movement = Owner->GetCharacterMovement();

        TScriptInterface<IGBCharacterMovementInterface>  CharacterMovementInterface = Owner;
        if (CharacterMovementInterface)
        {
            CrouchSpeedCoefficient = CharacterMovementInterface->GetCrouchSpeedCoefficient();
            BaseWalkSpeed = CharacterMovementInterface->GetBaseWalkSpeed();
            BaseSprintSpeed = CharacterMovementInterface->GetBaseSprintSpeed();
        }
    }

    if (BaseWalkSpeed <= 1.f || BaseSprintSpeed <= 1.f)
    {
        FTimerDelegate BaseSpeedDelegate;
        BaseSpeedDelegate.BindUObject(this, &UGBPlayerAnimInstance::InitBaseSpeed);
        GetWorld()->GetTimerManager().SetTimer(SetBaseSpeedTimer, BaseSpeedDelegate, 1.f, false);
    }
}
