#pragma once

UENUM(BlueprintType)
enum class EGBFieldItemType : uint8
{
    NONE,
    Item,
    Weapon,
    Equipment,
};

UENUM(BlueprintType)
enum class EGBItemType : uint8
{
    Posion,
};

UENUM(BlueprintType)
enum class EGBWeaponType : uint8
{
    Sword,
    Bow,
    Wand,
    Mace,
};

UENUM(BlueprintType)
enum class EGBEquipmentType : uint8
{
    top,
    bottoms,
};
