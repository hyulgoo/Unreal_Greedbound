#pragma once

UENUM(BlueprintType)
enum class EGBStanceState : uint8
{
    Stand,
    Crouch,
    Count
};

UENUM(BlueprintType)
enum class EGBStopState : uint8
{
    Stop,
    Move,
    Count
};

UENUM(BlueprintType)
enum class EGBMoveState : uint8
{
    Walk,
    Sprint,
    Count
};

UENUM(BlueprintType)
enum class EGBCombatState : uint8
{
    Idle,
    Attack,
    Knockback,
    Dead,
};

UENUM(BlueprintType)
enum class EGBDirection : uint8 
{
    NoReact,
    Forward,
    Left,
    Right,
    Backward
};

UENUM(BlueprintType)
enum class EGBSkillType : uint8 
{
    Skill_01,
    Skill_02,
    Skill_03,
    Skill_04,
    Count,
};
