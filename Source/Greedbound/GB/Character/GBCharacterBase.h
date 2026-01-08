// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/GBCharacterMovementInterface.h"
#include "Interface/GBCharacterCombatInterface.h"
#include "GameplayTagContainer.h"
#include "Define/GBCollision.h"
#include "GBCharacterBase.generated.h"

class UGameplayAbility;
class UAbilitySystemComponent;
class UGBAbilitySystemComponent;
class UGBMovementStateComponent;
class UGBCombatComponent;
class UGBCombatData;
class UGameplayEffect;
struct FGameplayAbilitySpec;
struct FGameplayEventData;

UCLASS()
class GREEDBOUND_API AGBCharacterBase : public ACharacter, public IAbilitySystemInterface, public IGBCharacterCombatInterface, public IGBCharacterMovementInterface
{
    GENERATED_BODY()

public:
    AGBCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
    virtual void                            BeginPlay() override;
    virtual void                            Tick(float DeltaTime) override;
    virtual void                            PossessedBy(AController* NewController) override;

    virtual UAbilitySystemComponent*        GetAbilitySystemComponent() const override final;

    template < typename T >
    T* GetAbilitySystemComponent() const { return Cast<T>(GetAbilitySystemComponent()); }

    virtual void                            SetAbilitySystemComponent(UGBAbilitySystemComponent* ASC);
    void                                    InitAbilitySystemComponent(AActor* OwnerActor);

    UGBMovementStateComponent*              GetMovementStateComponent() const;

    bool                                    IsPlayingMontage(UAnimMontage* Montage);

    // CharacterMovementInterface
    virtual void                            SetCharacterStanceState(const EGBStanceState NewState) override final;
    virtual void                            SetCharacterStopState(const EGBStopState NewState) override;
    virtual void                            SetCharacterMoveState(const EGBMoveState NewState) override final;
    virtual EGBStanceState                  GetCharacterStanceState() const override final;
    virtual EGBStopState                    GetCharacterStopState() const override final;
    virtual EGBMoveState                    GetCharacterMoveState() const override final;
    virtual float                           GetCrouchSpeedCoefficient() const final;
    virtual float                           GetBaseWalkSpeed() const final;
    virtual float                           GetBaseSprintSpeed() const final;

    void                                    UpdateSpeed();

    // IGBCharacterCombatInterface
    virtual void                            SetCombatState(const EGBCombatState NewState) override final;
    virtual EGBCombatState                  GetCombatState() const override final;
    virtual void                            BeRagdoll() override final;
    virtual void                            EquipWeapon(AGBWeapon* NewWeapon) override final;
    virtual UStaticMeshComponent*           GetWeaponMesh() const override;

    void                                    OnSprintEndEventRecieved(const FGameplayEventData* Payload);
    void                                    OnAttackEventRecieved(const FGameplayEventData* Payload);
    virtual void                            OnMovementStateTagChanged(const FGameplayTag Tag, const int32 Count);

    virtual FName                           GetAttackProfile() const { return GBCOLLISION_PROFILE_BLOCKALL; }

    virtual void                            LoadCharacterData();

private:
    UFUNCTION(NetMulticast, Reliable)
    void                                    Multicast_BeRagdoll();

private:
    UPROPERTY(Transient)
    TObjectPtr<UGBAbilitySystemComponent>   AbilitySystemComponent;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UGBMovementStateComponent>   MovementStateComponent;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UGBCombatComponent>          Combat;

    UPROPERTY()
    TSubclassOf<UGameplayEffect>            WeaponEquipGameplayEffectClass;
    
    UPROPERTY(EditDefaultsOnly, meta = (AllowedTypes = "CharacterData"))
    FPrimaryAssetId                         CharacterDataAssetId;
};
