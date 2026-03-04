// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GB/UI/InGame/GBActivatableBaseWidget.h"
#include "GBInventoryScreenWidget.generated.h"

class UCommonTileView;
class UGBTabListWidgetBase;
class UGBInventoryComponent;
class UGBInventoryDataRegistry;

UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class GREEDBOUND_API UGBInventoryScreenWidget : public UGBActivatableBaseWidget
{
	GENERATED_BODY()

public:
    UGBInventoryDataRegistry*               GetOrCreateDataRegistry();
    void                                    RefreshContainerItemList();
    void                                    RefreshBagItemList();
    //void                                  RefreshQuickSlotList();
    //void                                  RefreshEquipmentItemList();

protected:
    // UserWidget Interface
    virtual void                            NativeOnInitialized() override final;

    // CommonActivatableWidget Interface
    virtual UWidget*                        NativeGetDesiredFocusTarget() const override final;
    virtual void                            NativeOnActivated() override final;

private:
    void                                    OnItemSlotAdded(int32 SlotIndex, FPrimaryAssetId Id, int32 Count);
    void                                    OnItemSlotChanged(int32 SlotIndex, FPrimaryAssetId Id, int32 Count);
    void                                    OnItemSlotRemoved(int32 SlotIndex, FPrimaryAssetId Id, int32 Count);
    void                                    OnTileViewItemHovered(UObject* InHoveredItem, bool bWasHovered);
    void                                    OnTileViewItemSelected(UObject* InSelectedItem);

private:
    UPROPERTY(Transient)
    TWeakObjectPtr<UGBInventoryComponent>   CachedInventoryComponent;

    UPROPERTY(Transient)
    TObjectPtr<UGBInventoryDataRegistry>    InventoryDataRegistry;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UGBTabListWidgetBase>        CTL_InventoryTabListWidget;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonTileView>             CTV_Container;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonTileView>             CTV_Bag;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UWidget>                     ItemDescriptionPopupWidget;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UWidget>                     ItemActionPopupWidget;  

};
