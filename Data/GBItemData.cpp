// Fill out your copyright notice in the Description page of Project Settings.

#include "GBItemData.h"
#include "Define/GBDefine.h"
#include "Components/Player/GBInventoryComponent.h"

bool FGBInventoryList::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
{
    return FFastArraySerializer::FastArrayDeltaSerialize<FGBInventorySlot, FGBInventoryList>(Slots, DeltaParams, *this);
}

void FGBInventoryList::PostReplicatiedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
    GB_VALID_CHECK(OwnerComponent);

    TArray<int32> Added;
    Added.Reserve(AddedIndices.Num());
    for (int32 Idx : AddedIndices)
    {
        Added.Add(Idx);
    }

    OwnerComponent->OnSlotsAdded.Broadcast(Added);
}

void FGBInventoryList::PostReplicatiedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
    GB_VALID_CHECK(OwnerComponent);

    TArray<int32> Changed;
    Changed.Reserve(ChangedIndices.Num());
    for (int32 Idx : ChangedIndices)
    {
        Changed.Add(Idx);
    }

    OwnerComponent->OnSlotsChanged.Broadcast(Changed);
}

void FGBInventoryList::PostReplicatiedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
    GB_VALID_CHECK(OwnerComponent);

    TArray<int32> Removed;
    Removed.Reserve(RemovedIndices.Num());
    for (int32 Idx : RemovedIndices)
    {
        Removed.Add(Idx);
    }

    OwnerComponent->OnSlotsRemoved.Broadcast(Removed);
}
