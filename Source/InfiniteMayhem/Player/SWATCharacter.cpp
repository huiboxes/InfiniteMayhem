// Fill out your copyright notice in the Description page of Project Settings.


#include "SWATCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Weapon/WeaponActor.h"
#include "../Components/FPSCharacterMovementComponent.h"
#include "../Components/CombatComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>

ASWATCharacter::ASWATCharacter(const FObjectInitializer& Initializer): Super(Initializer.SetDefaultSubobjectClass<UFPSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) {

	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetOffset.Z = 50;

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
		CameraBoomSocketYOffset = 90;
		CameraBoomSocketZOffset = 60;
		CameraXOffset = 80;

		break;
	default:
		break;
	}
	CurrentState = State;
}

void ASWATCharacter::Accelerate() {
	if (Speed > 0) CameraXOffset -= 50; // 镜头后拉的效果
	
	bIsAcceleration = true;
}

void ASWATCharacter::UnAccelerate() {
	
	if(Speed != 0) CameraXOffset += 50;
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

void ASWATCharacter::IronsightButtonPressed() {
	if (!IsHoldWeapon()) return;
	UGameplayStatics::PlaySound2D(GetWorld(), RaiseArmSound);
	bAiming = true;
}

void ASWATCharacter::IronsightButtonReleased() {
	UGameplayStatics::PlaySound2D(GetWorld(), HolsterSound);
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

void ASWATCharacter::ToggleFire() {
	if (!CombatComp || !CombatComp->EquippedWeapon) return;
	if (!CombatComp->IsFiring()) { // 装备着武器时开枪，其它状态停止开枪
		CombatComp->EquippedWeapon->StartFire();
	} else {
		CombatComp->EquippedWeapon->StopFire();
	}
}

void ASWATCharacter::ReloadWeaponButtonPressed() {
	if (!IsHoldWeapon()) return;
	CombatComp->EquippedWeapon->ReloadWeapon();
}

void ASWATCharacter::UpdateCameraTargetPos(float DeltaTime) { // 更新相机偏移信息
	if (!CameraBoom || !MainCamera) return;

	CameraBoom->SocketOffset.Y = FMath::FInterpTo(CameraBoom->SocketOffset.Y, CameraBoomSocketYOffset, DeltaTime, 10);
	CameraBoom->SocketOffset.Z = FMath::FInterpTo(CameraBoom->SocketOffset.Z, CameraBoomSocketZOffset, DeltaTime, 10);

	float CameraX = FMath::FInterpTo(MainCamera->GetRelativeLocation().X, CameraXOffset, DeltaTime, 10);
	MainCamera->SetRelativeLocation(FVector(CameraX, 0, 0));
}

void ASWATCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	AimOffset(DeltaTime);
	UpdateCameraTargetPos(DeltaTime);
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
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ASWATCharacter::ToggleFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ASWATCharacter::ToggleFire);
	PlayerInputComponent->BindAction(TEXT("ReloadWeapon"), IE_Pressed, this, &ASWATCharacter::ReloadWeaponButtonPressed);

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

bool ASWATCharacter::IsFiring() {
	return CombatComp && CombatComp->IsFiring();
}

USpringArmComponent* ASWATCharacter::GetCameraBoom() {
	return CameraBoom;
}

AWeaponActor* ASWATCharacter::GetEquippedWeapon() {
	if (!CombatComp || !CombatComp->EquippedWeapon) return nullptr;
	return CombatComp->EquippedWeapon;
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

void ASWATCharacter::EnableEquiping() {
	if (!bIsEquiping) {
		bIsEquiping = true;

	}
}


void ASWATCharacter::AimOffset(float DeltaTime) {
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
	bool bIsInAir = IsInAir();
	

	FRotator Rot = GetControlRotation() - GetActorRotation();

	AO_Pitch = Rot.Pitch;
	AO_Yaw = Rot.Yaw;

	FQuat Qua = ActorToWorld().InverseTransformRotation(GetControlRotation().Quaternion());
	AO_Pitch = Qua.Rotator().Pitch;
	AO_Yaw = Qua.Rotator().Yaw;

}



