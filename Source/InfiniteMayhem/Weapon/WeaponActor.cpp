// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"
#include "../Player/SWATCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

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
	AmmonCurrent = AmmonMaxCounter;
}

void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponActor::HandleFire() {
	if (AmmonCurrent <= 0) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EmptySound, GetActorLocation());
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());
	AmmonCurrent--;
}

void AWeaponActor::ChangeWeaponState(EWeaponState State) {

	switch (State) {
	case EWeaponState::EWS_Initial:
		break;
	case EWeaponState::EWS_Equipped:
		SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		break;
	case EWeaponState::EWS_Drop:
		SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
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
	ChangeWeaponState(EWeaponState::EWS_Firing);
	HandleFire();
}

void AWeaponActor::StopFire() {
	ChangeWeaponState(EWeaponState::EWS_Equipped);
}


void AWeaponActor::ReloadWeapon() {
	
	ASWATCharacter* Player = Cast<ASWATCharacter>(GetOwner());
	if (!Player) return;
	
	FName SectionName = "Default";
	if (Player->IsCrouched()) {
		SectionName = "CrouchReload";
	}
	Player->PlayAnimMontage(ReloadMontage, 1, SectionName);
	AmmonCurrent = AmmonMaxCounter;


}
