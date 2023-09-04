// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"
#include "../Player/SWATCharacter.h"
#include "Bullet.h"
#include "Magazine.h"

#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/WidgetComponent.h"
#include "Curves/CurveFloat.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	
	// 枪栓位置箭头，用于确定抛壳特效位置
	GunBoltArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("GunBoltArrow"));
	GunBoltArrow->SetVisibility(false);
	GunBoltArrow->SetupAttachment(RootComponent);
	
	// 拾取提示 UI
	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
	PickupWidget->SetVisibility(false);

}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	if (MagNum) {
		AmmonCurrent = AmmonMaxCounter;
		MagNum--;

		if (MagClass) {
			MagComp = GetWorld()->SpawnActor<AMagazine>(MagClass);
			MagComp->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Gun_magazinesocket"));
		}
	}
}

void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AWeaponActor::ChangeWeaponState(EWeaponState State) {
	switch (State) {
	case EWeaponState::EWS_Initial:
		break;
	case EWeaponState::EWS_Standby:
		break;
	case EWeaponState::EWS_Equipped:
		//SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		break;
	case EWeaponState::EWS_Drop:
		//SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
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
	FVector MuzzlePos = MuzzleTransform.GetLocation();

	// 获得枪口到目标位置的方向，用于纠正射击位置
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

void AWeaponActor::HandleFire() {
	
	if (AmmonCurrent <= 0) { // 播放没子弹时的枪声并结束开枪
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EmptySound, GetActorLocation());
		StopFire();
		return;
	}

	// 执行蓝图中的扩展方法
	OnWeaponFire.Broadcast();

	// 普通枪声
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());

	// 获取枪口位置信息
	MuzzleTransform = WeaponMesh->GetSocketTransform(TEXT("Gun_Muzzle"));
	MuzzleTransform.SetScale3D(FVector(.3f, .3f, .3f));

	// 播放开火特效
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlameFX, MuzzleTransform);

	// 播放抛壳特效
	FTransform GunBoltArrowTransform = GunBoltArrow->GetComponentTransform();

	// 抛出的空弹壳粒子碰到地面时播放声音
	UParticleSystemComponent* EmptyShellPSC = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShellEjectionFX, GunBoltArrowTransform);
	EmptyShellPSC->OnParticleCollide.AddDynamic(this, &ThisClass::OnEmptyShellCollide);

	// 播放武器开火动画
	GetMesh()->PlayAnimation(FireAnim, false);

	// 射出子弹
	FireTheAmmon();
	AmmonCurrent--;
}

void AWeaponActor::OnEmptyShellCollide(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity, FVector Direction, FVector Normal, FName BoneName, UPhysicalMaterial* PhysMat){
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShellBounceSound, Location);
}


void AWeaponActor::StartFire() {
	if (!CanFire()) return;
	ChangeWeaponFireState(EWeaponFireState::EWS_Firing);
	if (bIsFullyAutomatic) {
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ThisClass::HandleFire, FiringRate, true, 0);
	} else {
		HandleFire();
		StopFire(); // 单发则提前停止
	}
	
}

void AWeaponActor::StopFire() {
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
	ChangeWeaponFireState(EWeaponFireState::EWS_Idle);
}


void AWeaponActor::ReloadWeapon() {

	ASWATCharacter* Player = Cast<ASWATCharacter>(GetOwner());
	if (CurrentFireState == EWeaponFireState::EWS_Reload || !Player || Player->IsEquiping() || AmmonCurrent >= AmmonMaxCounter || MagNum <= 0) return;
	
	bReloading = true;
	ChangeWeaponFireState(EWeaponFireState::EWS_Reload);

	FName SectionName = TEXT("Default");
	if (Player->IsCrouched()) {
		SectionName = TEXT("CrouchReload");
	}
	Player->PlayAnimMontage(ReloadMontage, 1, SectionName);
}


void AWeaponActor::ReloadAmmonOver() {
	bReloading = false;
	MagComp->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Gun_magazinesocket"));
	AmmonCurrent = AmmonMaxCounter;

	ChangeWeaponFireState(EWeaponFireState::EWS_Idle);
	
}

void AWeaponActor::RemoveMag() {
	ASWATCharacter* Player = Cast<ASWATCharacter>(GetOwner());
	if (!Player) return;

	MagComp->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("S_Mag"));
	GetWorldTimerManager().SetTimer(RemoveMagTimerHandle, MagComp, &AMagazine::DetachMag, 0.2f, false);
}


void AWeaponActor::GenerateMag() {
	ASWATCharacter* Player = Cast<ASWATCharacter>(GetOwner());
	if (!Player) return;

	MagComp = GetWorld()->SpawnActor<AMagazine>(MagClass);
	MagComp->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("S_Mag"));

	MagNum--;
}

bool AWeaponActor::CanFire() {
	ASWATCharacter* Player = Cast<ASWATCharacter>(GetOwner());
	if (!Player) return false;

	return bCanFire && !Player->IsEquiping() && !bReloading;
}

void AWeaponActor::WeaponDestroy() {
	MagComp->Destroy();
	Destroy();
}

USkeletalMeshComponent* AWeaponActor::GetMesh() {
	return WeaponMesh;
}
