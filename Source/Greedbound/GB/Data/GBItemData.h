// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Define/GBItemTypes.h"
#include "GBItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
    GENERATED_BODY()

public:
    FItemData() = default;

public:
    UPROPERTY(EditDefaultsOnly)
    EGBFieldItemType                        ItemType = EGBFieldItemType::NONE;
    
    UPROPERTY(EditDefaultsOnly)
    TMap<EGBItemType, TSubclassOf<AActor>>  ItemDatas;
};

UCLASS()
class GREEDBOUND_API UGBItemData : public UPrimaryDataAsset
{
    GENERATED_BODY()
    
public:
    UPROPERTY(EditDefaultsOnly)
    TArray<FItemData> FieldItemData;
};
