// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GBCharacterBase.h"
#include "Define/GBCollision.h"
#include "GBPlayerCharacter.generated.h"

class UInputComponent;
class UGBInputData;
class UGBInputComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UGBEscapeHandlerComponent;
struct FInputActionValue;
struct FGameplayEventData;
enum class EGBCharacterClassType : uint8;

UCLASS()
class GREEDBOUND_API AGBPlayerCharacter : public AGBCharacterBase
{
    GENERATED_BODY()

public:
    AGBPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    void                                    SetCharacterClassType(const EGBCharacterClassType NewClassType);

protected:
    virtual void                            BeginPlay() override;
    virtual void                            Tick(float DeltaTime) override;
    virtual void                            PossessedBy(AController* NewController) override final;
    virtual void                            OnRep_PlayerState() override final;

    // CharacterMovementInterface
    virtual void                            SetCharacterStopState(const EGBStopState NewState) override final;

    virtual  FName                          GetAttackProfile() const { return GBCOLLISION_PROFILE_PLAYERAOE; }

private:
    void                                    UpdateStopState();
    void                                    SprintContinue(const EGBStopState PrevState, const EGBStopState NewState);
    virtual void                            LoadCharacterData() override final;

private:
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UCameraComponent>            Camera;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<USpringArmComponent>         SpringArm;
        
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Escape", Meta = (AllowPrivateAccess))
    TObjectPtr<UGBEscapeHandlerComponent>   EscapeComponent;

    UPROPERTY(EditDefaultsOnly)
    EGBCharacterClassType                   ClassType;
};
