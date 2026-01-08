// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GBFieldItem.generated.h"

class USphereComponent;
enum class EGBFieldItemType : uint8;

UCLASS()
class GREEDBOUND_API AGBFieldItem : public AActor
{
    GENERATED_BODY()
    
public:    
    AGBFieldItem(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
    UFUNCTION()
    void                                OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, const int32 OtherBodyIndex, const bool bFromSweep, const FHitResult& SweepResult);

private:
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UStaticMeshComponent>    Mesh;
    
    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<USphereComponent>        Sphere;
    
    UPROPERTY(EditDefaultsOnly)
    EGBFieldItemType                    FieldItemType;
};
