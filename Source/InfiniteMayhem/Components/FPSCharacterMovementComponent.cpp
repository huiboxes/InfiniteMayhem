// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacterMovementComponent.h"
#include "../Player/SWATCharacter.h"

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
	bool IsCrouched = Player->IsCrouched();
	bool IsAcceleration = Player->IsAcceleration();

	switch (State) {
	case ESWATState::ESS_NORMAL:
		RunSpeed = IsCrouched ? CrouchMaxRunSpeed : NormalMaxRunSpeed;
		WalkSpeed = IsCrouched ? CrouchMaxWalkSpeed : NormalMaxWalkSpeed;
		break;

	case ESWATState::ESS_RILFE:
		RunSpeed = IsCrouched ? CrouchMaxRunSpeed : RilfeMaxRunSpeed;
		WalkSpeed = IsCrouched ? CrouchMaxWalkSpeed : RilfeMaxWalkSpeed;
		break;

	default:
		break;
	}

	MaxSpeed = IsAcceleration ? RunSpeed : WalkSpeed;

	return MaxSpeed;
}
