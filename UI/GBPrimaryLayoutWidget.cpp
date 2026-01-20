// Fill out your copyright notice in the Description page of Project Settings.

#include "GBPrimaryLayoutWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Define/GBDefine.h"
#include "Define/GBTags.h"
#include "UI/GBUIFunctionLibrary.h"
#include "UI/InGame/PlayerInfo/GBPlayerHUDWidget.h"
#include "UI/InGame/AsyncAction/GBAsyncAction_PushSoftWidget.h"
#include "AbilitySystem/Attribute/GBHealthAttributeSet.h"
#include "AbilitySystem/Attribute/GBSpeedAttributeSet.h"
#include "AbilitySystem/GBGameplayAbilityHelper.h"

UCommonActivatableWidgetContainerBase* UGBPrimaryLayoutWidget::FindWidgetStackByTag(const FGameplayTag& InTag) const
{
    GB_CONDITION_CHECK_WITH_RETURN(RegisteredWidgetStackMap.Contains(InTag), nullptr);
    return RegisteredWidgetStackMap.FindRef(InTag);
}

UGBPlayerHUDWidget* UGBPrimaryLayoutWidget::GetHUDWidget() const
{
    return HUDWidget;
}

void UGBPrimaryLayoutWidget::SetHUDVisible(bool bVisibie)
{
    GB_NULL_CHECK(HUDWidget);
    HUDWidget->SetVisibility(bVisibie ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UGBPrimaryLayoutWidget::SetHUDWidget(UAbilitySystemComponent* InASC)
{
    GB_NULL_CHECK(InASC);

    AbilitySystemComponent = InASC;

    GB_NULL_CHECK(HUDWidgetClass);

    HUDWidget = CreateWidget<UGBPlayerHUDWidget>(GetWorld(), HUDWidgetClass);
    HUDWidget->InitializeAttributeInfo(AbilitySystemComponent); 

    TArray<TPair<FGameplayTag, UTexture2D*>> SkillIconInfoList;

    TArray<FGameplayAbilitySpec>& Abilities = AbilitySystemComponent->GetActivatableAbilities();
    uint8 Skillndex = 1;
    for (const FGameplayAbilitySpec& AbilitySpec : Abilities)
    {
        UTexture2D* IconTexture = FGBGameplayAbilityHelper::GetSkillAbilityIcon(AbilitySpec.Ability);
        if (IconTexture)
        {
            const FGameplayTag& SkillTag = GetSkillTagByIndex(Skillndex);
            SkillIconInfoList.Add({ SkillTag, IconTexture });
            ++Skillndex;
        }
    }
        
    HUDWidget->SetSkillIconTexture(SkillIconInfoList);

    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGBHealthAttributeSet::GetCurrentHealthAttribute())
        .AddUObject(HUDWidget, &UGBPlayerHUDWidget::OnHealthAttributeChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGBHealthAttributeSet::GetMaxHealthAttribute())
        .AddUObject(HUDWidget, &UGBPlayerHUDWidget::OnHealthAttributeChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGBSpeedAttributeSet::GetCurrnetStaminaAttribute())
        .AddUObject(HUDWidget, &UGBPlayerHUDWidget::OnSpeedAttributeChanged);
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UGBSpeedAttributeSet::GetMaxStaminaAttribute())
        .AddUObject(HUDWidget, &UGBPlayerHUDWidget::OnSpeedAttributeChanged);

    const FGameplayTagContainer& BuffTags = FGBGameplayAbilityHelper::GetAllChildTag(GBTag::State_Buff);
    const FGameplayTagContainer& DebuffTags = FGBGameplayAbilityHelper::GetAllChildTag(GBTag::State_Debuff);
    const FGameplayTagContainer& CooldownTags = FGBGameplayAbilityHelper::GetAllChildTag(GBTag::State_Cooldown);

    for (const FGameplayTag BuffTag : BuffTags)
    {
        AbilitySystemComponent->RegisterGameplayTagEvent(BuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(HUDWidget, &UGBPlayerHUDWidget::OnBuffStateChanged);
    }

    for (const FGameplayTag DebuffTag : DebuffTags)
    {
        AbilitySystemComponent->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(HUDWidget, &UGBPlayerHUDWidget::OnBuffStateChanged);
    }

    for (const FGameplayTag CooldownTag : CooldownTags)
    {
        AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(HUDWidget, &UGBPlayerHUDWidget::OnSkillCooldownChanged);
    }

    AbilitySystemComponent->RegisterGameplayTagEvent(GBTag::State_EquipState_Equipped, EGameplayTagEventType::NewOrRemoved).AddUObject(HUDWidget, &UGBPlayerHUDWidget::OnSkillInfoChanged);

    HUDWidget->AddToViewport();
}

void UGBPrimaryLayoutWidget::RegisterWidgetStack(UPARAM(meta = (Categories = "UI.WidgetStack")) FGameplayTag InTag, UWidget* InWidgetStack)
{
    // 에디터타임이 아닐 때만 등록
    if (IsDesignTime())
    {
        return;
    }

    if (RegisteredWidgetStackMap.Contains(InTag) == false)
    {
        UCommonActivatableWidgetContainerBase* ActivatableWidgetContainer = Cast<UCommonActivatableWidgetContainerBase>(InWidgetStack);
        GB_NULL_CHECK(ActivatableWidgetContainer);

        RegisteredWidgetStackMap.Add(InTag, ActivatableWidgetContainer);
    }
}

TOptional<FUIInputConfig> UGBPrimaryLayoutWidget::GetDesiredInputConfig() const
{
    return FUIInputConfig(ECommonInputMode::All, EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown, EMouseLockMode::LockOnCapture, true);
}

bool UGBPrimaryLayoutWidget::NativeOnHandleBackAction()
{
    UGBAsyncAction_PushSoftWidget* PushAction = UGBAsyncAction_PushSoftWidget::PushSoftWidget(this, GetOwningPlayerController(), 
                                                    UGBUIFunctionLibrary::GetSoftWidgetClassByTag(GBTag::UI_Widget_MainMenuScreen), GBTag::UI_WidgetStack_GameMenu);
    if (PushAction)
    {
        PushAction->Activate();
    }

    return true;
}

FGameplayTag UGBPrimaryLayoutWidget::GetSkillTagByIndex(uint8 Index) const
{
    switch (Index)
    {
    case 1:
        return GBTag::Trigger_Common_Skill01;
    case 2:
        return GBTag::Trigger_Common_Skill02;
    case 3:
        return GBTag::Trigger_Common_Skill03;
    case 4:
        return GBTag::Trigger_Common_Skill04;
    default:
        GB_LOG(TEXT("Invalid Index : %i"), Index)
        break;
    }

    return FGameplayTag::EmptyTag;
}
