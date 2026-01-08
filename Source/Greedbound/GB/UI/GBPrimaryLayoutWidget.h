// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI\InGame\GBActivatableBaseWidget.h"
#include "GameplayTagContainer.h"
#include "Input/UIActionBindingHandle.h"
#include "GBPrimaryLayoutWidget.generated.h"

class UCommonActivatableWidgetContainerBase;
class UAbilitySystemComponent;
class UTexture2D;
class UGBPlayerHUDWidget;

UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class GREEDBOUND_API UGBPrimaryLayoutWidget : public UGBActivatableBaseWidget
{
    GENERATED_BODY()
public:
    UCommonActivatableWidgetContainerBase*  FindWidgetStackByTag(const FGameplayTag& InTag) const;
    UGBPlayerHUDWidget*                     GetHUDWidget() const;
    void                                    SetHUDVisible(bool bVisibie);
    void                                    SetHUDWidget(UAbilitySystemComponent* InASC);

protected:
    UFUNCTION(BlueprintCallable)
    void                                    RegisterWidgetStack(UPARAM(meta = (Categories = "UI.WidgetStack"))FGameplayTag InTag, UWidget* InWidgetStack);

private:
    virtual TOptional<FUIInputConfig>       GetDesiredInputConfig() const override final;
    virtual bool                            NativeOnHandleBackAction() override final;

    FGameplayTag                            GetSkillTagByIndex(uint8 Index) const;

private:
    UPROPERTY(Transient)
    TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>>   RegisteredWidgetStackMap;

    UPROPERTY(Transient)
    TObjectPtr<UAbilitySystemComponent>     AbilitySystemComponent;

    UPROPERTY(Transient)
    TObjectPtr<UGBPlayerHUDWidget>          HUDWidget;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGBPlayerHUDWidget>         HUDWidgetClass;
};
