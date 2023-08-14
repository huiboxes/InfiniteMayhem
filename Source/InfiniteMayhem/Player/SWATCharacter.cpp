// Fill out your copyright notice in the Description page of Project Settings.


#include "SWATCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ASWATCharacter::ASWATCharacter() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCamera->SetupAttachment(CameraBoom);


}

// Called when the game starts or when spawned
void ASWATCharacter::BeginPlay() {
	Super::BeginPlay();


}

void ASWATCharacter::MoveForward(float Value) {

	AddMovementInput(GetActorForwardVector() * Value);
}

void ASWATCharacter::MoveRight(float Value) {
	AddMovementInput(GetActorRightVector() * Value);
}

// Called every frame
void ASWATCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASWATCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASWATCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASWATCharacter::MoveRight);
}

