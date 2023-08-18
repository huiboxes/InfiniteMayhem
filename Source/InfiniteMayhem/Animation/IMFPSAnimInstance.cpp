// Fill out your copyright notice in the Description page of Project Settings.


#include "IMFPSAnimInstance.h"
#include "../Player/SWATCharacter.h"

void UIMFPSAnimInstance::NativeUpdateAnimation(float DeltaTime) {
	Super::NativeUpdateAnimation(DeltaTime);
	if (!Player) return;

	Speed = Player->GetVelocity().Size2D();

	bIsCrouched = Player->IsCrouched();
	bIsInAir = Player->IsInAir();
	bAiming = Player->IsAiming();
	bHoldWeapon = Player->IsHoldWeapon();

	FVector MoveDir = Player->GetVelocity();
	MoveDir.Normalize();

	Direction = CalculateDirection(Player->GetVelocity(), FRotator(0, Player->GetControlRotation().Yaw,0)); // 获取给定向量和旋转之间的夹角

	// 计算当前的瞄准方向值
	FRotator Rot = Player->GetControlRotation() - Player->GetActorRotation();
	AO_Pitch = Rot.Pitch;
	AO_Yaw = Rot.Yaw;

	FQuat Qua = Player->ActorToWorld().InverseTransformRotation(Player->GetControlRotation().Quaternion());
	AO_Pitch = Qua.Rotator().Pitch;
	AO_Yaw = Qua.Rotator().Yaw;

}

void UIMFPSAnimInstance::NativeBeginPlay() {
	Super::NativeBeginPlay();
	Player = Cast<ASWATCharacter>(TryGetPawnOwner());
}
