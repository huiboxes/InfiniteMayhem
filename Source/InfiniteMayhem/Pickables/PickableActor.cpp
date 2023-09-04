// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

APickableActor::APickableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionProfileName(TEXT("Custom"));
}

void APickableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APickableActor::EnableOutlineDisplay_Implementation() {
	ItemMesh->SetRenderCustomDepth(true);
}

void APickableActor::DisableOutlineDisplay_Implementation() {
	ItemMesh->SetRenderCustomDepth(false);
}


void APickableActor::Pickup_Implementation(AActor* _Owner) { // 默认情况捡到了就销毁 Actor
	GetWorldTimerManager().SetTimer(PickupTimerHandle, this, &APickableActor::DestroySelf, 0.2f, false);
}

void APickableActor::DestroySelf() {
	Destroy();
}
