// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"
#include "../Player/SWATCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

AWeaponActor::AWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeaponActor::OnSphereBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AWeaponActor::OnSphereEndOverlap);
	
	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
	PickupWidget->SetVisibility(false);

}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponActor::ChangeWeaponState(EWeaponState State) {

	switch (State) {
	case EWeaponState::EWS_INITIAL:
		break;
	case EWeaponState::EWS_EQUIPPED:
		break;
	case EWeaponState::EWS_DROP:
		break;
	case EWeaponState::EWS_MAX:
		break;
	default:
		break;
	}
	
	CurrentState = State;
}

void AWeaponActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	ASWATCharacter* Player = Cast<ASWATCharacter>(OtherActor);
	if (Player) {
		Player->SetOverlappingWeapon(this);
	}
}

void AWeaponActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	ASWATCharacter* Player = Cast<ASWATCharacter>(OtherActor);
	if (Player) {
		Player->SetOverlappingWeapon(nullptr);
	}
}

void AWeaponActor::ShowPickupWidget(bool bShowWidget) {
	if (PickupWidget) {
		PickupWidget->SetVisibility(bShowWidget);
	}
}


void AWeaponActor::StartFire() {
	ChangeWeaponState(EWeaponState::EWS_FIRING);
}

void AWeaponActor::StopFire() {
	ChangeWeaponState(EWeaponState::EWS_EQUIPPED);
}
