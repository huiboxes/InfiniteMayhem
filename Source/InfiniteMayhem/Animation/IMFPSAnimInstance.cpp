// Fill out your copyright notice in the Description page of Project Settings.


#include "IMFPSAnimInstance.h"
#include "../Weapon/WeaponActor.h"

void UIMFPSAnimInstance::NativeUpdateAnimation(float DeltaTime) {
	Super::NativeUpdateAnimation(DeltaTime);
	if (!Player) return;

	Speed = Player->GetSpeed();

	bIsCrouched = Player->IsCrouched();
	bIsInAir = Player->IsInAir();
	bAiming = Player->IsAiming();
	bHoldWeapon = Player->IsHoldWeapon();
	bFire = Player->IsFiring();
	bEquiping = Player->IsEquiping();

	FVector MoveDir = Player->GetVelocity();
	MoveDir.Normalize();

	Direction = CalculateDirection(Player->GetVelocity(), FRotator(0, Player->GetControlRotation().Yaw,0)); // 获取给定向量和旋转之间的夹角

	AO_Yaw = Player->GetAO_Yaw();
	AO_Pitch = Player->GetAO_Pitch();
}

void UIMFPSAnimInstance::NativeBeginPlay() {
	Super::NativeBeginPlay();
	Player = Cast<ASWATCharacter>(TryGetPawnOwner());
}


void UIMFPSAnimInstance::AnimNotify_ReloadOver() {
	if (Player && Player->IsHoldWeapon()) {
		Player->GetEquippedWeapon()->ReloadAmmonOver();
	}
}
