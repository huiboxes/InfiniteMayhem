// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SWATCharacter.generated.h"

UENUM(BlueprintType)
enum class ESWATState :uint8 {
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_Rilfe UMETA(DisplayName = "Rilfe"),

	ESS_MAX
};

UCLASS()
class INFINITEMAYHEM_API ASWATCharacter : public ACharacter {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASWATCharacter(const class FObjectInitializer& Initializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void Accelerate();
	void UnAccelerate();
	void CrouchButtonPressed();
	void CrouchButtonReleased();
	void IronsightButtonPressed();
	void IronsightButtonReleased();
	void PickupButtonPressed();
	void SwitchWeaponButtonPressed();
	void ToggleFire();

	void UpdateCameraTargetPos(float DeltaTime);

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsAcceleration();

	bool IsHoldWeapon();
	
	bool IsCrouched() { return bIsCrouched; };

	bool IsInAir();

	bool IsAiming();

	bool IsFiring();

	ESWATState GetCurrentState() { return CurrentState; };

	void ChangeState(ESWATState State);


	class USpringArmComponent* GetCameraBoom();

	void SetOverlappingWeapon(class AWeaponActor* Weapon);

	class UCombatComponent* GetCombatComp() { return CombatComp; };

protected:

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* MainCamera;
	
	UPROPERTY(VisibleAnywhere)
	class AWeaponActor* OverlappingWeapon;

	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* CombatComp;

	bool bIsAcceleration = false;
	
	bool bIsCrouched = false;

	bool bAiming = false;

	ESWATState CurrentState = ESWATState::ESS_Normal;

	float CameraBoomSocketYOffset = 30;
	float CameraBoomTargetZOffset;
	float CameraXOffset;

};
