// Fill out your copyright notice in the Description page of Project Settings.


#include "IMFPSAnimInstance.h"
#include "../Weapon/Weapon.h"
#include "../Components/CombatComponent.h"

void UIMFPSAnimInstance::NativeUpdateAnimation(float DeltaTime) {
	Super::NativeUpdateAnimation(DeltaTime);
	if (!Player) return;

	UpdateStatusFromPlayer();
	//UpdateLeftHandTransform();

}

void UIMFPSAnimInstance::NativeBeginPlay() {
	Super::NativeBeginPlay();
	Player = Cast<ASWATCharacter>(TryGetPawnOwner());
}


void UIMFPSAnimInstance::UpdateStatusFromPlayer() {
	Speed = Player->GetSpeed();

	bIsCrouched = Player->IsCrouched();
	bIsInAir = Player->IsInAir();
	bAiming = Player->IsAiming();
	bHoldWeapon = Player->IsHoldWeapon();
	bFire = Player->IsFiring();
	bIsEquiping = Player->IsEquiping();
	bPicking = Player->IsPicking();

	Standing = Player->GetStanding();
	Equiping = bIsEquiping ? 1 : 0;

	FVector MoveDir = Player->GetVelocity();
	MoveDir.Normalize();

	Direction = CalculateDirection(Player->GetVelocity(), FRotator(0, Player->GetControlRotation().Yaw, 0)); // 获取给定向量和旋转之间的夹角

	AO_Yaw = Player->GetAO_Yaw();
	AO_Pitch = Player->GetAO_Pitch();

}

void UIMFPSAnimInstance::AnimNotify_ReloadOver() {
	if (Player && Player->IsHoldWeapon()) {
		Player->GetEquippedWeapon()->ReloadAmmonOver();
	}
}

void UIMFPSAnimInstance::AnimNotify_PullOutWeapon() {
	if (Player) {
		Player->GetCombatComp()->ChangeEquippedWeapon();
	}
}

void UIMFPSAnimInstance::AnimNotify_SwitchWeaponOver() {
	if (Player) {
		Player->DisableEquiping();
	}
}

void UIMFPSAnimInstance::AnimNotify_RemoveMag() {
	if (Player) {
		Player->GetEquippedWeapon()->RemoveMag();
	}

}

void UIMFPSAnimInstance::AnimNotify_GenerateMag() {
	if (Player) {
		Player->GetEquippedWeapon()->GenerateMag();
	}
}

void UIMFPSAnimInstance::AnimNotify_LeftFootStep() {
	if (Player) {
		Player->PlayLeftFootStep();
	}
}

void UIMFPSAnimInstance::AnimNotify_RightFootStep() {
	if (Player) {
		Player->PlayRighttFootStep();
	}
}

void UIMFPSAnimInstance::UpdateLeftHandTransform() {
	if (Player->IsHoldWeapon()) {
		// 获取左手插槽的变换信息
		LeftHandTransform = Player->GetEquippedWeapon()->GetMesh()->GetSocketTransform(TEXT("S_ForeEndHand"), ERelativeTransformSpace::RTS_World);
			
		FVector OutPosition; // 存储输出的位置信息
		FRotator OutRotation; // 存储输出的旋转信息
		// 将右手的世界变换信息转换为骨骼的相对位置（相对与左手插槽）

		Player->GetMesh()->TransformToBoneSpace(TEXT("RightHand"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition); // 设置左手插槽相对与右手的位置
		LeftHandTransform.SetRotation(FQuat(OutRotation)); // 设置左手插槽相对于右手的旋转
	}
}
