// Fill out your copyright notice in the Description page of Project Settings.


#include "SWATCharacter.h"
#include "../Weapon/WeaponActor.h"
#include "../Components/FPSCharacterMovementComponent.h"
#include "../Components/CombatComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/ArrowComponent.h"
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
		CameraXOffset = 50;

		break;
	default:
		break;
	}
	CurrentState = State;
}

void ASWATCharacter::Accelerate() {
	if (Speed >= 10) CameraXOffset -= 50; // 镜头后拉的效果
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
	if (IsHoldWeapon()) UGameplayStatics::PlaySound2D(GetWorld(), HolsterSound);
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

void ASWATCharacter::SwitchFireModeButtonPressed() {
	if (CombatComp && CombatComp->EquippedWeapon) {
		CombatComp->EquippedWeapon->SwitchFireMode();
	}
}

void ASWATCharacter::ToggleFire() {
	if (!CombatComp || !CombatComp->EquippedWeapon) return;
	if (!CombatComp->IsFiring()) { // 装备着武器时开枪，其它状态停止开枪
		CombatComp->EquippedWeapon->StartFire();
	}
}

void ASWATCharacter::StopFire() {
	if (!CombatComp || !CombatComp->EquippedWeapon) return;
	CombatComp->EquippedWeapon->StopFire();
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
	PlayerInputComponent->BindAction(TEXT("SwitchFireMode"), IE_Pressed, this, &ASWATCharacter::SwitchFireModeButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ASWATCharacter::ToggleFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ASWATCharacter::StopFire);
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

void ASWATCharacter::FootstepJudgment(FVector ToeLoc) {
	FVector Start = ToeLoc;
	FVector End = Start;
	End.Z -= 50.f; // 向下偏移

	FHitResult OutHit;
	TArray<AActor*> ActorsToIgnore; // 如果有要忽略的 Actor，可以添加到这个数组
	ActorsToIgnore.Add(Controller->GetPawn());

	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true)) { // 自定义的 Hit 通道
		UPhysicalMaterial* mat = OutHit.PhysMaterial.Get();
		FVector HitLoc = OutHit.Location;
		
		switch (mat->SurfaceType) {
			case EPhysicalSurface::SurfaceType1: // 踩到了 Metal
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), OnMetalSound, OutHit.Location);
				break;
			case EPhysicalSurface::SurfaceType2: // 踩到了 Gravel
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), OnGravelSound, OutHit.Location);
				break;
			case EPhysicalSurface::SurfaceType3: // 踩到了 Conocrete
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), OnConcreteSound, OutHit.Location);
				break;
			case EPhysicalSurface::SurfaceType4: // 踩到了 Wood
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), OnWoodSound, OutHit.Location);
				break;
			case EPhysicalSurface::SurfaceType5: // 踩到了 Rock
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), OnRockSound, OutHit.Location);
				break;
			case EPhysicalSurface::SurfaceType6: // 踩到了 Grass
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), OnGrassSound, OutHit.Location);
				break;
			default: // 默认踩到泥土
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitDefaultSound, OutHit.Location);
				break;
		}
	}

}

void ASWATCharacter::PlayLeftFootStep() {
	FVector LeftToeBaseLoc = GetMesh()->GetSocketLocation(TEXT("LeftToeBase"));
	FootstepJudgment(LeftToeBaseLoc);
}

void ASWATCharacter::PlayRighttFootStep() {
	FVector LeftToeBaseLoc = GetMesh()->GetSocketLocation(TEXT("RightToeBase"));
	FootstepJudgment(LeftToeBaseLoc);
}

