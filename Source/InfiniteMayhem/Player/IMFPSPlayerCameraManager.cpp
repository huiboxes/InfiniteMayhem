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
			if (Player->IsCrouched() && !bCrouch) {
				CurrentCameraPosOffsetZ = 46;
			} else if (!Player->IsCrouched() && bCrouch) {
				CurrentCameraPosOffsetZ = -46;
			}

			bCrouch = Player->IsCrouched();
			CurrentCameraPosOffsetZ = FMath::FInterpTo(CurrentCameraPosOffsetZ, 0, DeltaTime, 10);
			Player->GetCameraBoom()->SetRelativeLocation(FVector(0, 0, CurrentCameraPosOffsetZ));
		}
	}

	Super::UpdateCamera(DeltaTime);

}
