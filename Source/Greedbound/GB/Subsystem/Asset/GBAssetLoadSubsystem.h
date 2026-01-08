// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "GBAssetLoadSubsystem.generated.h"

class UGBAbilitySystemComponent;
enum class EGBCharacterClassType : uint8;

UCLASS()
class GREEDBOUND_API UGBAssetLoadSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    static UGBAssetLoadSubsystem*   Get(const UObject* WorldContextObject);

    void                            LoadCharacterDataByAssetID(const FPrimaryAssetId& AssetToLoad, UGBAbilitySystemComponent* GBASC);
    void                            LoadCharacterDataByTag(const FGameplayTag& AssetNameTag, UGBAbilitySystemComponent* GBASC);
    void                            LoadCharacterDataByClassType(const EGBCharacterClassType ClassType, UGBAbilitySystemComponent* GBASC);
};
