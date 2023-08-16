// Fill out your copyright notice in the Description page of Project Settings.


#include "SWATCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Components/FPSCharacterMovementComponent.h"

// Sets default values
ASWATCharacter::ASWATCharacter(const FObjectInitializer& Initializer): Super(Initializer.SetDefaultSubobjectClass<UFPSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) {

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
	FVector Dir = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X); // 构建旋转矩阵
	AddMovementInput(Dir * Value);
}

void ASWATCharacter::MoveRight(float Value) {

	FVector Dir = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y); // 构建旋转矩阵
	AddMovementInput(Dir * Value);

}

void ASWATCharacter::LookUp(float Value) {
	AddControllerPitchInput(Value);
}

void ASWATCharacter::Turn(float Value) {
	AddControllerYawInput(Value);
}

void ASWATCharacter::Accelerate() {
	bIsAcceleration = true;
}

void ASWATCharacter::UnAccelerate() {
	bIsAcceleration = false;
}

void ASWATCharacter::CrouchButtonPressed() {
	bIsCrouched = true;
	Crouch();

}


void ASWATCharacter::CrouchButtonReleased() {
	bIsCrouched = false;
	UnCrouch();

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
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ASWATCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ASWATCharacter::LookUp);
	PlayerInputComponent->BindAction(TEXT("Accelerate"), IE_Pressed, this, &ASWATCharacter::Accelerate);
	PlayerInputComponent->BindAction(TEXT("Accelerate"), IE_Released, this, &ASWATCharacter::UnAccelerate);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ASWATCharacter::CrouchButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &ASWATCharacter::CrouchButtonReleased);

}

bool ASWATCharacter::IsAcceleration() {
	FVector MoveDir = GetVelocity();
	MoveDir.Normalize();
	return FVector::DotProduct(GetActorForwardVector(), MoveDir) > 0.9 && bIsAcceleration;
}

