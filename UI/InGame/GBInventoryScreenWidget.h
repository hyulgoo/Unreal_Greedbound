// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GB/UI/InGame/GBActivatableBaseWidget.h"
#include "GBInventoryScreenWidget.generated.h"

UCLASS()
class GREEDBOUND_API UGBInventoryScreenWidget : public UGBActivatableBaseWidget
{
	GENERATED_BODY()

public:
    // UserWidget Interface
    virtual void        NativeOnInitialized() override final;

    // CommonActivatableWidget Interface
    virtual void        NativeOnActivated() override final;
    virtual void        NativeOnDeactivated() override final;
    virtual UWidget*    NativeGetDesiredFocusTarget() const override final;
	
private:
    UPROPERTY(Transient)
    TObjectPtr<AActor>  CachedOwner;
};
