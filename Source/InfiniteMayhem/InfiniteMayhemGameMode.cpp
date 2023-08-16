// Fill out your copyright notice in the Description page of Project Settings.


#include "InfiniteMayhemGameMode.h"
#include "Player/SWATCharacter.h"
#include "IMFPSPlayerController.h"

AInfiniteMayhemGameMode::AInfiniteMayhemGameMode() {
	static ConstructorHelpers::FClassFinder<ASWATCharacter> SWATClass(TEXT("Blueprint'/Game/InfiniteMayhem/Blueprints/Player/BP_SWAT.BP_Swat_C'"));
	DefaultPawnClass = SWATClass.Class;

	PlayerControllerClass = AIMFPSPlayerController::StaticClass();


}
