// Fill out your copyright notice in the Description page of Project Settings.

#include "GBFieldItem.h"
#include "Components/SphereComponent.h"

AGBFieldItem::AGBFieldItem(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FieldItemMesh"));
    SetRootComponent(Mesh);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    Sphere->SetupAttachment(Mesh);
    Sphere->OnComponentBeginOverlap.AddDynamic(this, &AGBFieldItem::OnSphereOverlap);

    // TODO FieldItem용 Profile 설정
    //Sphere->SetCollisionProfileName()
}

void AGBFieldItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, const int32 OtherBodyIndex, const bool bFromSweep, const FHitResult& SweepResult)
{
    // TODO 
    // 상호작용 UI 팝업
    // 플레이어가 해당 상태에서 조작 시 ItemType에 따라 Item생성 및 캐릭터에 세팅
}
