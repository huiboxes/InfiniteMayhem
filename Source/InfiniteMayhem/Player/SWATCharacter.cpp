// Fill out your copyright notice in the Description page of Project Settings.


#include "SWATCharacter.h"
#include "../IMFPSPlayerController.h"
#include "../Weapon/Weapon.h"
#include "../Components/FPSCharacterMovementComponent.h"
#include "../Components/CombatComponent.h"
#include "../Interface/IPickableInterface.h"
#include "../Pickables/PickableActor.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "EngineUtils.h"
#include "../CameraShake/HitMatineeCameraShake.h"
#include "MatineeCameraShake.h"

ASWATCharacter::ASWATCharacter(const FObjectInitializer& Initializer): Super(Initializer.SetDefaultSubobjectClass<UFPSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) {

	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	

	CameraBoom->TargetOffset.Z = 50;

	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCamera->SetupAttachment(CameraBoom);
	
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComp"));
	// 设置胶囊体忽略摄像机的碰撞
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	// 设置网格体忽略摄像机的碰撞
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);

	PawnNoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>("PawnNoiseEmitterComponent");
}

void ASWATCharacter::BeginPlay() {
	Super::BeginPlay();
	if (Controller) {
		ActorsToIgnoreInFootstep.Add(Controller->GetPawn());
		ActorsToIgnoreInPickRange.Add(Controller->GetPawn());
	}
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
	case ESWATState::ESS_Dead:
		Die();
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
	SetStanding(0.f);
	Crouch();
}


void ASWATCharacter::CrouchButtonReleased() {
	bIsCrouched = false;
	SetStanding(1.f);
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

void ASWATCharacter::PickupButtonPressed() { // 只对实现了 IIPickableInterface 的 Actor 有反应
	FHitResult OutHit;
	if (PickRangeDetection(OutHit)) {
		bPicking = true;
		GetWorldTimerManager().SetTimer(PickupTimerHandle, this, &ASWATCharacter::CancelPicking, 0.6f, false);
		
		IIPickableInterface::Execute_Pickup(OutHit.GetActor(), this);
		APickableActor* HitItem = Cast<APickableActor>(OutHit.GetActor());
		if (HitItem) HitItem->ShowPickupWidget(false);
		OnPickup.Broadcast();
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
		PawnNoiseEmitterComponent->MakeNoise(this, 1.f, GetActorLocation());
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

 bool ASWATCharacter::PickRangeDetection(FHitResult& Hit) {
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (!CameraManager) return false;
	FVector CameraLoc = CameraManager->GetCameraLocation();
	FVector End = CameraManager->GetActorForwardVector() * 450 + CameraLoc;
	
	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), CameraLoc, End, 10, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel4), false, ActorsToIgnoreInPickRange, EDrawDebugTrace::None, Hit, true)) {
		
		/*UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *Hit.GetActor()->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"), *Hit.Location.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Hit Normal: %s"), *Hit.Normal.ToString());*/
		return true;
	}


	return false;
}

 bool ASWATCharacter::OutlineDisplayDetection() {
	 FHitResult OutHit;
	 bool bHasPickableItem = PickRangeDetection(OutHit);
	 if (bHasPickableItem) { // 检测到了可拾取物
		 bOutlineDisplayDetectionReset = true;

		 AActor* CurrentPickableItem = OutHit.GetActor();
		 if (DetectedPickableItem != CurrentPickableItem) { // 保证永远只有一个可拾取物品开启了描边
			 if (DetectedPickableItem) { // 关闭上次扫描到的可拾取物的描边
				 // UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *CurrentPickableItem->GetName());
				 // UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"), *CurrentPickableItem->GetActorLocation().ToString());
				 IIPickableInterface::Execute_DisableOutlineDisplay(DetectedPickableItem);
				 IIPickableInterface::Execute_EnableOutlineDisplay(CurrentPickableItem);
				 //IIPickableInterface::
				 APickableActor* OldItem = Cast<APickableActor>(DetectedPickableItem);
				 APickableActor* CurrentItem = Cast<APickableActor>(CurrentPickableItem);
				 if (OldItem && CurrentItem) {
					 OldItem->ShowPickupWidget(false);
					 CurrentItem->ShowPickupWidget(true);
				 }

			 }
			 DetectedPickableItem = CurrentPickableItem; // 重新设置
		 }
	 } else { // 未检测到可拾取物，将所有可拾取物关闭描边显示
		 if (bOutlineDisplayDetectionReset) {
			 for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr) { // 遍历所有 AActor
				 AActor* CurrentActor = *ActorItr;

				 // 检查当前Actor是否实现了 UIPickableInterface
				 if (CurrentActor->GetClass()->ImplementsInterface(UIPickableInterface::StaticClass())) {
					 IIPickableInterface::Execute_DisableOutlineDisplay(CurrentActor);
					 DetectedPickableItem = nullptr;

					 APickableActor* OldItem = Cast<APickableActor>(DetectedPickableItem);
					 APickableActor* CurrentItem = Cast<APickableActor>(CurrentActor);
					 if (OldItem) OldItem->ShowPickupWidget(false);
					 if (CurrentItem) CurrentItem->ShowPickupWidget(false);
				 }
			 }

			bOutlineDisplayDetectionReset = false;
		 }
	 }

	 return false;
 }

void ASWATCharacter::UpdateCameraTargetPos(float DeltaTime) { // 更新相机偏移信息
	if (!CameraBoom || !MainCamera) return;

	CameraBoom->SocketOffset.Y = FMath::FInterpTo(CameraBoom->SocketOffset.Y, CameraBoomSocketYOffset, DeltaTime, 10);
	CameraBoom->SocketOffset.Z = FMath::FInterpTo(CameraBoom->SocketOffset.Z, CameraBoomSocketZOffset, DeltaTime, 10);

	float CameraX = FMath::FInterpTo(MainCamera->GetRelativeLocation().X, CameraXOffset, DeltaTime, 10);
	MainCamera->SetRelativeLocation(FVector(CameraX, 0, 0));
}

void ASWATCharacter::UpdateHealth(float DeltaTime) {
	Health = FMath::FInterpTo(Health, TargetHelth, DeltaTime, 10);
}

void ASWATCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	AimOffset(DeltaTime);
	OutlineDisplayDetection();
	UpdateHealth(DeltaTime);
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


float ASWATCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	if (IsDead()) return 0;

	TargetHelth = Health - DamageAmount;
	
	if (TargetHelth < 0) TargetHelth = 0;
	if (IsDead()) {
		ChangeState(ESWATState::ESS_Dead);
	} 

	UGameplayStatics::PlaySound2D(GetWorld(), HitSound);
	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (PCM) {
		UMatineeCameraShake::StartMatineeCameraShake(PCM, UHitMatineeCameraShake::StaticClass());
	}

	int32 Index = FMath::RandRange(0, HitMontageArray.Num() - 1);
	PlayAnimMontage(HitMontageArray[Index]);
	return 1;
}

void ASWATCharacter::Die() {
	GetMesh()->SetSimulatePhysics(true);
	MainCamera->SetActive(false);
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (CombatComp) CombatComp->EnableWeaponSimulatePhysics();
	
	OnPlayerDead.Broadcast();

	/*GetWorld()->GetTimerManager().SetTimer(SawThePlayerTimerHandle, [=]() {
		Destroy();
	}, 15.f, false);*/
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

AWeapon* ASWATCharacter::GetEquippedWeapon() {
	if (!CombatComp || !CombatComp->EquippedWeapon) return nullptr;
	return CombatComp->EquippedWeapon;
}


void ASWATCharacter::SetOverlappingWeapon(AWeapon* Weapon) {
	//if (OverlappingWeapon) { // 检查上一次的是否还存在
	//	OverlappingWeapon->ShowPickupWidget(false);
	//}
	//
	//OverlappingWeapon = Weapon;
	//if (OverlappingWeapon) {
	//	OverlappingWeapon->ShowPickupWidget(true);
	//}

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
	AO_Pitch = FMath::Clamp(Qua.Rotator().Pitch, -75.f, 75.f);
	AO_Yaw = FMath::Clamp(Qua.Rotator().Yaw, -75.f, 75.f);

}

void ASWATCharacter::FootstepHandle(FVector ToeLoc) {
	FVector End = ToeLoc;
	End.Z -= 50.f; // 向下偏移

	FHitResult OutHit;

	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), ToeLoc, End, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2), false, ActorsToIgnoreInFootstep, EDrawDebugTrace::None, OutHit, true)) { // 自定义的 Hit 通道
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
	FootstepHandle(LeftToeBaseLoc);
}

void ASWATCharacter::PlayRighttFootStep() {
	FVector LeftToeBaseLoc = GetMesh()->GetSocketLocation(TEXT("RightToeBase"));
	FootstepHandle(LeftToeBaseLoc);
}

float ASWATCharacter::GetAmmonCurrent() {
	AWeapon* Weapon = GetEquippedWeapon();
	if (Weapon) {
		return Weapon->GetAmmonCurrent();
	}
	return 0;
}

float ASWATCharacter::GetMagNum() {
	AWeapon* Weapon = GetEquippedWeapon();
	if (Weapon) {
		return Weapon->GetMagNum();
	}
	return 0;
}

