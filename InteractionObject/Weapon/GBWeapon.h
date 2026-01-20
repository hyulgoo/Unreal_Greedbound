// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "GBWeapon.generated.h"

class UCapsuleComponent;
class UNiagaraComponent;

UCLASS()
class GREEDBOUND_API AGBWeapon : public AActor
{
    GENERATED_BODY()

public:    
    AGBWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
    
    const FName&                        GetAttachSocketName() const;
    void                                SetActiveTrailEffect(const bool bActive);

private:
    virtual void                        GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION()
    void                                OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void                                OnRep_TrailEffectActive();

private:
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UStaticMeshComponent>    Mesh;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UCapsuleComponent>       Capsule;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UNiagaraComponent>       TrailParticle;

    UPROPERTY(ReplicatedUsing = OnRep_TrailEffectActive)
    uint8                               bTrailEffectActive : 1;

    UPROPERTY(EditDefaultsOnly)
    FName                               AttachSocketName;

    UPROPERTY(EditDefaultsOnly, meta = (Categories = "Character.Player"))
    FGameplayTag                        ClassTag;
};
