// Fill out your copyright notice in the Description page of Project Settings.

#include "GBPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Define/GBAnimSectionName.h"
#include "Define/GBDefine.h"
#include "Define/GBTags.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/GBAbilitySystemComponent.h"
#include "Components/Player/GBEscapeHandlerComponent.h"
#include "Components/GBMovementStateComponent.h"
#include "Character/Player/GBPlayerState.h"
#include "Subsystem/Asset/GBAssetLoadSubsystem.h"

AGBPlayerCharacter::AGBPlayerCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
    SpringArm->SetupAttachment(GetRootComponent());
    SpringArm->SetUsingAbsoluteRotation(true);
    SpringArm->SetRelativeLocation(FVector(0.f , 0.f, 75.f));
    SpringArm->SocketOffset = FVector(0.f, 50.f, 0.f);
    SpringArm->TargetArmLength = 300.f;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

    GetCharacterMovement()->bUseControllerDesiredRotation = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    EscapeComponent = CreateDefaultSubobject<UGBEscapeHandlerComponent>(TEXT("EscapeHandlerComponent"));
}


void AGBPlayerCharacter::SetCharacterClassType(const EGBCharacterClassType NewClassType)
{
    ClassType = NewClassType;

    LoadCharacterData();
}

void AGBPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocallyControlled())
    {
        GB_BACKEND_LOG(Warning, TEXT("[BeginPlay-Character] It is Locally Controlled : %s"), *GetName());
    }
    else
    {
        GB_BACKEND_LOG(Warning, TEXT("[BeginPlay-Character] It is remotely controlled: %s"), *GetName());
    }
}

void AGBPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateStopState();
}

void AGBPlayerCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    AGBPlayerState* GBPS = GetPlayerState<AGBPlayerState>();
    if (GBPS)
    {
        UGBAbilitySystemComponent* GBASC = GBPS->GetAbilitySystemComponent<UGBAbilitySystemComponent>();
        GB_NULL_CHECK(GBASC);

        SetAbilitySystemComponent(GBASC);
        InitAbilitySystemComponent(GBPS);
    }

    UpdateSpeed();
}

void AGBPlayerCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    GB_CONDITION_CHECK_WITHOUT_LOG(HasAuthority() == false);

    AGBPlayerState* GBPS = GetPlayerState<AGBPlayerState>();
    if (GBPS)
    {
        UGBAbilitySystemComponent* GBASC = GBPS->GetAbilitySystemComponent<UGBAbilitySystemComponent>();
        GB_NULL_CHECK(GBASC);
        SetAbilitySystemComponent(GBASC);
        InitAbilitySystemComponent(GBPS);
    }
}

void AGBPlayerCharacter::SetCharacterStopState(const EGBStopState NewState)
{
    const EGBStopState PrevState = GetMovementStateComponent()->GetCharacterStopState();
    GetMovementStateComponent()->SetCharacterStopState(NewState);

    if (PrevState != NewState && GetLocalRole() == ROLE_AutonomousProxy)
    {
        SprintContinue(PrevState, NewState);
    }
}

void AGBPlayerCharacter::UpdateStopState()
{
    UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
    GB_NULL_CHECK(CharacterMovementComponent);

    const FVector Velocity = GetVelocity();
    const float GroundSpeed = Velocity.Size2D();
    const bool bIsMoving = GroundSpeed > GBMovementConstants::MoveThreshold;

    if (bIsMoving)
    {
        SetCharacterStopState(EGBStopState::Move);
        CharacterMovementComponent->bUseControllerDesiredRotation = true;
    }
    else
    {
        SetCharacterStopState(EGBStopState::Stop);

        const bool bIsMovable = GetMovementStateComponent()->IsMovable();
        if (GetCombatState() <= EGBCombatState::Attack && bIsMovable)
        {
            UGBMovementStateComponent* MovementState = GetMovementStateComponent();
            if (GetLocalRole() >= ROLE_AutonomousProxy)
            {
                const float DeltaYaw = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation()).Yaw;

                FName SectionName = NAME_None;

                if (DeltaYaw > GBMovementConstants::TurnThresholdForAutonomous)
                {
                    SectionName = GBANIMSECTIONNAME_TURN_RIGHT;
                }
                else if (DeltaYaw < -GBMovementConstants::TurnThresholdForAutonomous)
                {
                    SectionName = GBANIMSECTIONNAME_TURN_LEFT;
                }

                UAnimMontage* TIPMontage = MovementState->GetTurnInPlaceMontage();
                if (TIPMontage)
                {
                    if (SectionName != NAME_None)
                    {
                        CharacterMovementComponent->bUseControllerDesiredRotation = true;
                        if (!IsPlayingMontage(TIPMontage))
                        {
                            PlayAnimMontage(TIPMontage, 1.f, SectionName);
                        }
                    }
                    else if (!IsPlayingMontage(TIPMontage))
                    {
                        CharacterMovementComponent->bUseControllerDesiredRotation = false;
                    }
                }
            }
            else
            {
                UAnimMontage* TIPMontage = GetMovementStateComponent()->GetTurnInPlaceMontage();

                const FRotator DeltaRotation = MovementState->GetDeltaRotation();
                const float YawDelta = DeltaRotation.Yaw;

                MovementState->SetLastRotation();

                if (!IsPlayingMontage(TIPMontage) && (FMath::Abs(YawDelta) >= GBMovementConstants::TurnThresholdForSimulatedProxy))
                {
                    const FName SectionName = YawDelta > 0.f ? GBANIMSECTIONNAME_TURN_RIGHT : GBANIMSECTIONNAME_TURN_LEFT;
                    PlayAnimMontage(TIPMontage, 1.f, SectionName);
                }
            }
        }
        else if (bIsMovable == false)
        {
            CharacterMovementComponent->bUseControllerDesiredRotation = false;
        }
    }
}

void AGBPlayerCharacter::SprintContinue(const EGBStopState PrevState, const EGBStopState NewState)
{
    if (PrevState == EGBStopState::Stop && NewState != EGBStopState::Stop)
    {
        UGBAbilitySystemComponent* GBASC = GetAbilitySystemComponent<UGBAbilitySystemComponent>();
        if (GBASC)
        {
            if (GBASC->HasMatchingGameplayTag(GBTag::Trigger_Common_Sprint))
            {
                GBASC->AbilityTagToggled(GBTag::Trigger_Common_Sprint);
            }
        }
    }
    else if (PrevState != EGBStopState::Stop && NewState == EGBStopState::Stop)
    {
        UGBAbilitySystemComponent* GBASC = GetAbilitySystemComponent<UGBAbilitySystemComponent>();
        if (GBASC)
        {
            const FGameplayTagContainer AssetTags(GBTag::Trigger_Common_Sprint);
            GBASC->CancelAbilities(&AssetTags);
        }
    }
}

void AGBPlayerCharacter::LoadCharacterData()
{
    UGBAssetLoadSubsystem::Get(this)->LoadCharacterDataByClassType(ClassType, GetAbilitySystemComponent<UGBAbilitySystemComponent>());
}

