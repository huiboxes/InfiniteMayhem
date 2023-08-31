// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"
#include "../Player/SWATCharacter.h"
#include "Bullet.h"

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
	ChangeWeaponFireState(EWeaponFireState::EWS_Firing);
	if (AmmonCurrent <= 0) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EmptySound, GetActorLocation());
		return;
	}
	FireTheAmmon();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());
	AmmonCurrent--;
}

void AWeaponActor::ChangeWeaponState(EWeaponState State) {
	switch (State) {
	case EWeaponState::EWS_Initial:
		break;
	case EWeaponState::EWS_Standby:
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

void AWeaponActor::ChangeWeaponFireState(EWeaponFireState State) {
	CurrentFireState = State;
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

void AWeaponActor::FireTheAmmon() {
	APlayerController* Pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!Pc) return;
	int32 Width = 0;
	int32 Height = 0;

	Pc->GetViewportSize(Width, Height);
	FVector STWPos;
	FVector STWDir;

	Pc->DeprojectScreenPositionToWorld(Width / 2.f, Height / 2.f, STWPos, STWDir);

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, STWPos, STWPos + STWDir * MaxShootDistance, ECC_Visibility)) {
		Projectile(Hit.ImpactPoint);
	}
}


void AWeaponActor::Projectile(FVector TargetPos) {
	// 从枪口发射射线
	FVector MuzzlePos = WeaponMesh->GetSocketLocation(TEXT("Gun_Muzzle"));
	
	FVector FireDir = (TargetPos - MuzzlePos).GetSafeNormal();
	
	if (BulletClass) {// 从枪口生成一个子弹
		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, MuzzlePos, FireDir.Rotation());
	}

}

void AWeaponActor::ShowPickupWidget(bool bShowWidget) {
	if (PickupWidget) {
		PickupWidget->SetVisibility(bShowWidget);
	}
}


void AWeaponActor::StartFire() {
	if (!CanFire()) return;
	HandleFire();
}

void AWeaponActor::StopFire() {
	ChangeWeaponFireState(EWeaponFireState::EWS_Idle);
}


void AWeaponActor::ReloadWeapon() {
	
	ASWATCharacter* Player = Cast<ASWATCharacter>(GetOwner());
	if (!Player || CurrentFireState == EWeaponFireState::EWS_Reload) return;

	ChangeWeaponFireState(EWeaponFireState::EWS_Reload);

	FName SectionName = TEXT("Default");
	if (Player->IsCrouched()) {
		SectionName = TEXT("CrouchReload");
	}
	Player->PlayAnimMontage(ReloadMontage, 1, SectionName);
}


void AWeaponActor::ReloadAmmonOver() {
	AmmonCurrent = AmmonMaxCounter;
	ChangeWeaponFireState(EWeaponFireState::EWS_Idle);
}

USkeletalMeshComponent* AWeaponActor::GetMesh() {
	return WeaponMesh;
}
