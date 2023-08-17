// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacterMovementComponent.h"
#include "../Player/SWATCharacter.h"
#include "Components/CapsuleComponent.h"


float UFPSCharacterMovementComponent::GetMaxSpeed() const {
	return GetPlayerCurrentSpeed();
}

float UFPSCharacterMovementComponent::GetPlayerCurrentSpeed() const {
	ASWATCharacter* Player = Cast<ASWATCharacter>(GetOwner());

	if (!Player) return Super::GetMaxSpeed();

	float MaxSpeed = 0;
	float RunSpeed = 0;
	float WalkSpeed = 0;

	ESWATState State = Player->GetCurrentState();
	bool bIsCrouched = Player->IsCrouched();
	bool bIsAcceleration = Player->IsAcceleration();
	bool bIsIronsight = Player->IsIronsight();

	switch (State) {
	case ESWATState::ESS_Normal:
		RunSpeed = bIsCrouched ? CrouchMaxRunSpeed : NormalMaxRunSpeed;
		WalkSpeed = bIsCrouched ? CrouchMaxWalkSpeed : NormalMaxWalkSpeed;
		break;

	case ESWATState::ESS_Rilfe:
		RunSpeed = bIsCrouched ? CrouchMaxRunSpeed : RilfeMaxRunSpeed;
		WalkSpeed = bIsCrouched ? CrouchMaxWalkSpeed : RilfeMaxWalkSpeed;
		break;

	default:
		break;
	}

	RunSpeed = bIsIronsight ? RunSpeed * 0.7 : RunSpeed;
	WalkSpeed = bIsIronsight ? WalkSpeed * 0.7 : WalkSpeed;

	MaxSpeed = bIsAcceleration ? RunSpeed : WalkSpeed;
	if (bIsAcceleration && bIsCrouched) {
		Player->GetCapsuleComponent()->SetCapsuleHalfHeight(70);
	} else if(!bIsAcceleration && bIsCrouched) {
		Player->GetCapsuleComponent()->SetCapsuleHalfHeight(50);
	}

	return MaxSpeed;
}
