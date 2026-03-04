// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InGame/Inventory/DataObjects/GBListDataObject_Item.h"

void UGBListDataObject_Item::SetItemData(FPrimaryAssetId ItemId, int32 ItemCount)
{
    if (ItemId.IsValid())
    {
        ItemData.ItemId = ItemId;
        ItemData.Count = ItemCount;

        NotifyListDataModified(this, EGBOptionsListDataModifyReason::DirectlyModified);
    }
}
