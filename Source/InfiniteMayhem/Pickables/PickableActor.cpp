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
	UE_LOG(LogTemp, Log, TEXT("2222222222  ~~~·"));
}

void APickableActor::DisableOutlineDisplay_Implementation() {
	ItemMesh->SetRenderCustomDepth(false);
	UE_LOG(LogTemp, Log, TEXT("1111111111111  ~~~·"));
}


void APickableActor::Pickup_Implementation() {
}
