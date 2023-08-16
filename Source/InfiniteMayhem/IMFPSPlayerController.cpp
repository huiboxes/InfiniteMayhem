// Fill out your copyright notice in the Description page of Project Settings.


#include "IMFPSPlayerController.h"
#include "Player/IMFPSPlayerCameraManager.h"

AIMFPSPlayerController::AIMFPSPlayerController() {
	PlayerCameraManagerClass = AIMFPSPlayerCameraManager::StaticClass();

}
