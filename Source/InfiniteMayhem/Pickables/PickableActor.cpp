// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableActor.h"
#include "../Player/SWATCharacter.h"
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

	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSphereEndOverlap);
}

void APickableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickableActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	ASWATCharacter* Player = Cast<ASWATCharacter>(OtherActor);
	if (Player) {
		//Player->SetOverlappingWeapon(this);
	}
}

void APickableActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	ASWATCharacter* Player = Cast<ASWATCharacter>(OtherActor);
	if (Player) {
		Player->SetOverlappingWeapon(nullptr);
	}
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
