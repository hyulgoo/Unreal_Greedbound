// Fill out your copyright notice in the Description page of Project Settings.

#include "GBInventoryScreenWidget.h"
#include "CommonTileView.h"
#include "Define/GBDefine.h"
#include "Data/GBItemData.h"
#include "Character/Player/GBPlayerState.h"
#include "Subsystem/UI/GBUISubsystem.h"
#include "Components/Player/GBInventoryComponent.h"
#include "UI/InGame/Component/GBTabListWidgetBase.h"
#include "UI/InGame/Inventory/GBInventoryDataRegistry.h"
#include "UI/InGame/Inventory/DataObjects/GBListDataObject_Item.h"
#include "UI/InGame/Options/DataObjects/GBListDataObject_Collection.h"

UGBInventoryDataRegistry* UGBInventoryScreenWidget::GetOrCreateDataRegistry()
{
    if (InventoryDataRegistry == nullptr)
    {
        InventoryDataRegistry = NewObject<UGBInventoryDataRegistry>();
        InventoryDataRegistry->InitInventoryDataRegistry(CachedInventoryComponent->GetInventoryMaxCount());
    }

    GB_NULL_CHECK_WITH_RETURN(InventoryDataRegistry, nullptr);

    return InventoryDataRegistry;
}

void UGBInventoryScreenWidget::RefreshContainerItemList()
{
    GB_NULL_CHECK(CTV_Container);

    // TODO 컨테이너에서 interact로 ItemList 가져오기
}

void UGBInventoryScreenWidget::RefreshBagItemList()
{
    GB_NULL_CHECK(CTV_Bag);

    UGBInventoryComponent* InventoryComponent = CachedInventoryComponent.Get();
    GB_NULL_CHECK(InventoryComponent);

    const int32 InventoryMaxCount = InventoryComponent->GetInventoryMaxCount();
    const TArray<FGBInventorySlot>& SlotList = InventoryComponent->GetItemLists();
    const TArray<UGBListDataObject_Base*>& DataList = GetOrCreateDataRegistry()->GetListSourceItemBySelectedTabID(FName("InventoryTabCollection"));
    GB_CONDITION_CHECK(SlotList.Num() == InventoryMaxCount && DataList.Num() == InventoryMaxCount);

    for (int32 Index = 0; Index < InventoryMaxCount; ++Index)
    {
        UGBListDataObject_Item* ItemDataObject = Cast<UGBListDataObject_Item>(DataList[Index]);
        GB_NULL_CHECK(ItemDataObject);
        ItemDataObject->SetItemData(SlotList[Index].ItemId, SlotList[Index].Count);
    }
}

void UGBInventoryScreenWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    
    // Inventory
    {
        GB_NULL_CHECK(CTL_InventoryTabListWidget);
        GB_NULL_CHECK(CTV_Bag);

        APlayerController* PlayerController = GetOwningPlayerController();
        GB_NULL_CHECK(PlayerController);

        AGBPlayerState* PlayerState = PlayerController->GetPlayerState<AGBPlayerState>();
        GB_NULL_CHECK(PlayerState);

        CachedInventoryComponent = PlayerState->GetInventoryComponent();
        GB_NULL_CHECK(CachedInventoryComponent);

        const TArray<UGBListDataObject_Base*>& DataList = GetOrCreateDataRegistry()->GetListSourceItemBySelectedTabID(FName("InventoryTabCollection"));
        CTV_Bag->SetListItems(DataList);

        CachedInventoryComponent->OnSlotsAdded.AddUObject(this, &UGBInventoryScreenWidget::OnItemSlotAdded);
        CachedInventoryComponent->OnSlotsChanged.AddUObject(this, &UGBInventoryScreenWidget::OnItemSlotChanged);
        CachedInventoryComponent->OnSlotsRemoved.AddUObject(this, &UGBInventoryScreenWidget::OnItemSlotRemoved);

        CTV_Bag->OnItemSelectionChanged().AddUObject(this, &UGBInventoryScreenWidget::OnTileViewItemSelected);
        CTV_Bag->OnItemIsHoveredChanged().AddUObject(this, &UGBInventoryScreenWidget::OnTileViewItemHovered);

        RefreshBagItemList();
    }

    // Map (TODO)
    {
        //CTL_InventoryTabListWidget->OnTabSelected.AddUnique(this, &UGBInventoryScreenWidget::OnInventoryTabSelected);
    }
}

UWidget* UGBInventoryScreenWidget::NativeGetDesiredFocusTarget() const
{
    UGBUISubsystem::Get(GetOwningLocalPlayer())->SetHUDVisible(false);

    UObject* SelectedObject = CTV_Bag->GetSelectedItem();
    if (SelectedObject)
    {
        UUserWidget* SelectedEntryWidget = CTV_Bag->GetEntryWidgetFromItem(SelectedObject);
        if (SelectedEntryWidget)
        {
            return SelectedEntryWidget;
        }
    }

    return CTV_Bag;
}

void UGBInventoryScreenWidget::NativeOnActivated()
{
    Super::NativeOnActivated();

    const TArray<UGBListDataObject_Collection*>& RegisteredTabCollections = GetOrCreateDataRegistry()->GetRegisteredOptionsTabCollections();
    for (UGBListDataObject_Collection* TabCollection : RegisteredTabCollections)
    {
        if (TabCollection)
        {
            const FName& TabId = TabCollection->GetDataID();
            CTL_InventoryTabListWidget->RequestRegisterTab(TabId, TabCollection->GetDataDisplayName());
        }
    }
}

void UGBInventoryScreenWidget::OnItemSlotAdded(int32 SlotIndex, FPrimaryAssetId Id, int32 Count)
{
}

void UGBInventoryScreenWidget::OnItemSlotChanged(int32 SlotIndex, FPrimaryAssetId Id, int32 Count)
{
    if (Id.IsValid() == false)
    {
        return;
    }

    UGBListDataObject_Item* ItemDataObject = Cast<UGBListDataObject_Item>(CTV_Bag->GetItemAt(SlotIndex));
    GB_NULL_CHECK(ItemDataObject);

    ItemDataObject->SetItemData(Id, Count);
}

void UGBInventoryScreenWidget::OnItemSlotRemoved(int32 SlotIndex, FPrimaryAssetId Id, int32 Count)
{   
}

void UGBInventoryScreenWidget::OnTileViewItemHovered(UObject* InHoveredItem, bool bWasHovered)
{
}

void UGBInventoryScreenWidget::OnTileViewItemSelected(UObject* InSelectedItem)
{
    //UGBListDataObject_Item* ItemDataobject = Cast<UGBListDataObject_Item>(InSelectedItem);
    //GB_NULL_CHECK(ItemDataobject);
}
