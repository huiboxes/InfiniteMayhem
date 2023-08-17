// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "IMFPSPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEMAYHEM_API AIMFPSPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()


protected:

	virtual void UpdateCamera(float DeltaTime) override;

	void SmoothingCrouch(class ASWATCharacter* Player, float DeltaTime);

	void Aiming(class ASWATCharacter* Player, float DeltaTime);

protected:

	float CurrentCameraPosOffsetZ = 0;

	bool bCrouch = false;
	
};
