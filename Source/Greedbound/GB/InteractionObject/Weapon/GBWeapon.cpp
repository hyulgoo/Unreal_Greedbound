
#include "GBWeapon.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "Net/UnrealNetwork.h"
#include "Interface/GBCharacterCombatInterface.h"
#include "Define/GBCollision.h"
#include "Define/GBSocketName.h"
#include "AbilitySystem/GBGameplayAbilityHelper.h"

AGBWeapon::AGBWeapon(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    Mesh->SetCollisionProfileName(GBCOLLISION_PROFILE_NOCOLLISION);
    SetRootComponent(Mesh);

    Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
    Capsule->SetupAttachment(RootComponent);
    Capsule->SetCollisionProfileName(GBCOLLISION_PROFILE_ITEM);
    Capsule->OnComponentBeginOverlap.AddDynamic(this, &AGBWeapon::OnSphereBeginOverlap);

    TrailParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailParticle"));
    TrailParticle->SetupAttachment(RootComponent, GBSOCKETNAME_ATTACKSOCKET01);
    TrailParticle->bAutoActivate = false;
}

float AGBWeapon::GetAttack() const
{
    return Attack;
}

const FName& AGBWeapon::GetAttachSocketName() const
{
    return AttachSocketName;
}

void AGBWeapon::SetActiveTrailEffect(const bool bActive)
{
    bTrailEffectActive = bActive;
}

void AGBWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AGBWeapon, bTrailEffectActive);
}

void AGBWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    TScriptInterface<IGBCharacterCombatInterface> CombatInterface = OtherActor;
    if (CombatInterface)
    {
        if (HasAuthority())
        {
            if (FGBGameplayAbilityHelper::HasMatchingTagGameplayTag(ClassTag, OtherActor))
            {
                CombatInterface->EquipWeapon(this);
                Capsule->SetCollisionProfileName(GBCOLLISION_PROFILE_NOCOLLISION);
            }
        }
    }
}

void AGBWeapon::OnRep_TrailEffectActive()
{
    if (bTrailEffectActive)
    {
        TrailParticle->Activate(false);
    }
    else
    {
        TrailParticle->Deactivate();
    }
}

