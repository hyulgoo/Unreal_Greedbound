// Fill out your copyright notice in the Description page of Project Settings.

#include "GBTA_MeleeAttack.h"
#include "Define/GBDefine.h"
#include "Define/GBCollision.h"
#include "KismetTraceUtils.h"
#include "Abilities/GameplayAbilityTargetActor_Trace.h"

AGBTA_MeleeAttack::AGBTA_MeleeAttack(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;
    SetActorTickEnabled(false);
}

void AGBTA_MeleeAttack::StartTargeting(UGameplayAbility* Ability)
{
    Super::StartTargeting(Ability);

    SetActorTickEnabled(true);

    if(StartLocation.SourceComponent)
    {
        TraceSocketNames = StartLocation.SourceComponent->GetAllSocketNames();
        for(const FName& SocketName : TraceSocketNames)
        {
            TraceSocketPrevLocations.Add(SocketName, StartLocation.SourceComponent->GetSocketLocation(SocketName));
        }
    }
}

void AGBTA_MeleeAttack::CancelTargeting()
{
    SetActorTickEnabled(false);

    AlreadyHitActors.Empty();
    TraceSocketNames.Empty();
    TraceSocketPrevLocations.Empty();
}

void AGBTA_MeleeAttack::Tick(float DeltaSeconds)
{
    GB_NULL_CHECK(StartLocation.SourceComponent);

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(StartLocation.SourceActor);

    UWorld* World = GetWorld();

    FGameplayTargetDataFilterHandle FilterHandle;
    FGameplayTargetDataFilter* NewFilter = new FGameplayTargetDataFilter();
    NewFilter->InitializeFilterContext(StartLocation.SourceActor);
    FilterHandle.Filter = TSharedPtr<FGameplayTargetDataFilter>(NewFilter);

    for (TPair<FName, FVector>& PrevSocketLocation : TraceSocketPrevLocations)
    {
        FHitResult HitResult;
        //FCollisionResponseParams;
        const FTransform& SocketTransform = StartLocation.SourceComponent->GetSocketTransform(PrevSocketLocation.Key);
        const FVector Start = PrevSocketLocation.Value;
        const FVector End    = StartLocation.SourceComponent->GetSocketLocation(PrevSocketLocation.Key);
        const float Radius = 5.f;
        
        AGameplayAbilityTargetActor_Trace::SweepWithFilter(HitResult, World, FilterHandle, Start, End, 
            SocketTransform.Rotator().Quaternion(), FCollisionShape::MakeSphere(Radius), GBCOLLISION_PROFILE_PLAYERAOE, Params);
        
#if ENABLE_DRAW_DEBUG

        if (bDebug)
        {
            DrawDebugSweptSphere(World, Start, End, TraceSize.X / 2.f, HitResult.bBlockingHit ? FColor::Red : FColor::Green, false, 0.3f);
        }
#endif // ENABLE_DRAW_DEBUG

        AActor* HitActor = HitResult.GetActor();
        if (HitResult.bBlockingHit && HitActor && AlreadyHitActors.Contains(HitActor) == false)
        {
            AlreadyHitActors.Add(HitActor);
            FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
            FGameplayAbilityTargetDataHandle TargetDataHandle;
            TargetDataHandle.Data.Add(TSharedPtr<FGameplayAbilityTargetData>(TargetData));

            TargetDataReadyDelegate.Broadcast(TargetDataHandle);
        }

        PrevSocketLocation.Value = End;
    }
}
