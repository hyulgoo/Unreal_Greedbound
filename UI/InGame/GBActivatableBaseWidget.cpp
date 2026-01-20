// Fill out your copyright notice in the Description page of Project Settings.

#include "GBActivatableBaseWidget.h"
#include "GameFramework/PlayerController.h"
#include "Input/CommonUIInputTypes.h"

APlayerController* UGBActivatableBaseWidget::GetOwningPlayerController()
{
    if (CachedOwningPC.IsValid() == false)
    {
        CachedOwningPC = GetOwningPlayer<APlayerController>();
    }

    return CachedOwningPC.IsValid() ? CachedOwningPC.Get() : nullptr;
}

TOptional<FUIInputConfig> UGBActivatableBaseWidget::GetDesiredInputConfig() const
{
    return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture, EMouseLockMode::DoNotLock, true);
}
