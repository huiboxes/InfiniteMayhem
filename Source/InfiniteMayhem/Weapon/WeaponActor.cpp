// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"
#include "Components/SphereComponent.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
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

