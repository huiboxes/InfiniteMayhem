// Fill out your copyright notice in the Description page of Project Settings.


#include "IMFPSPlayerCameraManager.h"
#include "../IMFPSPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "SWATCharacter.h"

void AIMFPSPlayerCameraManager::UpdateCamera(float DeltaTime) {
	

	AIMFPSPlayerController* Pc = Cast<AIMFPSPlayerController>(PCOwner);
	if (Pc) {
		ASWATCharacter* Player = Cast<ASWATCharacter>(Pc->GetPawn());
		if (Player) {
			SmoothingCrouch(Player, DeltaTime);
			Aiming(Player, DeltaTime);
		}
	}

	Super::UpdateCamera(DeltaTime);

}

void AIMFPSPlayerCameraManager::SmoothingCrouch(ASWATCharacter* Player, float DeltaTime) {
	bool bIsCrouched = Player->IsCrouched();

	if (bIsCrouched && !bCrouch) {
		CurrentCameraPosOffsetZ = 46;
	} else if (!bIsCrouched && bCrouch) {
		CurrentCameraPosOffsetZ = -46;
	}

	bCrouch = bIsCrouched;
	CurrentCameraPosOffsetZ = FMath::FInterpTo(CurrentCameraPosOffsetZ, 0, DeltaTime, 10);
	Player->GetCameraBoom()->SetRelativeLocation(FVector(0, 0, CurrentCameraPosOffsetZ));

}

void AIMFPSPlayerCameraManager::Aiming(ASWATCharacter* Player, float DeltaTime) {
	if (!Player->IsHoldWeapon()) return;
	float CurrentFOV = 0;

	bool bIsIronsight = Player->IsIronsight();
	if (bIsIronsight) {
		CurrentFOV = 60;
	} else {
		CurrentFOV = 90;
	}

	DefaultFOV = FMath::FInterpTo(DefaultFOV, CurrentFOV, DeltaTime, 10);
	SetFOV(DefaultFOV);
}
