// Fill out your copyright notice in the Description page of Project Settings.


#include "SWATCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Weapon/WeaponActor.h"
#include "../Components/FPSCharacterMovementComponent.h"
#include "../Components/CombatComponent.h"

ASWATCharacter::ASWATCharacter(const FObjectInitializer& Initializer): Super(Initializer.SetDefaultSubobjectClass<UFPSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) {

	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCamera->SetupAttachment(CameraBoom);

	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));
}

// Called when the game starts or when spawned
void ASWATCharacter::BeginPlay() {
	Super::BeginPlay();


}

void ASWATCharacter::MoveForward(float Value) {
	if (Controller != nullptr && Value != 0.f) {
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void ASWATCharacter::MoveRight(float Value) {
	if (Controller != nullptr && Value != 0.f) {
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}

}

void ASWATCharacter::LookUp(float Value) {
	AddControllerPitchInput(Value);
}

void ASWATCharacter::Turn(float Value) {
	AddControllerYawInput(Value);
}

void ASWATCharacter::ChangeState(ESWATState State) {
	switch (State) {
	case ESWATState::ESS_Normal:
		break;
	case ESWATState::ESS_Rilfe:
		break;
	default:
		break;
	}
	CurrentState = State;
}

void ASWATCharacter::Accelerate() {
	if (IsHoldWeapon()) return;
	bIsAcceleration = true;
}

void ASWATCharacter::UnAccelerate() {
	bIsAcceleration = false;
}


void ASWATCharacter::CrouchButtonPressed() {
	if (!IsHoldWeapon()) return ;
	bIsCrouched = true;
	Crouch();
}


void ASWATCharacter::CrouchButtonReleased() {
	bIsCrouched = false;
	UnCrouch();

}

void ASWATCharacter::IronsightButtonPressed() {
	if (!IsHoldWeapon()) return;
	bAiming = true;
}

void ASWATCharacter::IronsightButtonReleased() {
	bAiming = false;
}

void ASWATCharacter::PickupButtonPressed() {
	if (CombatComp) {
		CombatComp->EquipWeapon(OverlappingWeapon);
	}
}

void ASWATCharacter::SwitchWeaponButtonPressed() {
	if (CombatComp) {
		CombatComp->SwitchWeapon();
	}
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
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ASWATCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Ironsight"), IE_Pressed, this, &ASWATCharacter::IronsightButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Ironsight"), IE_Released, this, &ASWATCharacter::IronsightButtonReleased);
	PlayerInputComponent->BindAction(TEXT("Pickup"), IE_Pressed, this, &ASWATCharacter::PickupButtonPressed);
	PlayerInputComponent->BindAction(TEXT("SwitchWeapon"), IE_Pressed, this, &ASWATCharacter::SwitchWeaponButtonPressed);

}

bool ASWATCharacter::IsAcceleration() {
	FVector MoveDir = GetVelocity();
	MoveDir.Normalize();
	return FVector::DotProduct(GetActorForwardVector(), MoveDir) > 0.9 && !bAiming && bIsAcceleration;
}

bool ASWATCharacter::IsHoldWeapon() {
	return CombatComp && CombatComp->EquippedWeapon;
}

bool ASWATCharacter::IsInAir() {
	return GetCharacterMovement()->IsFalling();
}

bool ASWATCharacter::IsAiming() {
	return bAiming;
}

USpringArmComponent* ASWATCharacter::GetCameraBoom() {
	return CameraBoom;
}

void ASWATCharacter::SetOverlappingWeapon(AWeaponActor* Weapon) {
	if (OverlappingWeapon) { // 检查上一次的是否还存在
		OverlappingWeapon->ShowPickupWidget(false);
	}
	
	OverlappingWeapon = Weapon;
	if (OverlappingWeapon) {
		OverlappingWeapon->ShowPickupWidget(true);
	}

}


