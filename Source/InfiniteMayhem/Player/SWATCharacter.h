// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SWATCharacter.generated.h"

UENUM(BlueprintType)
enum class ESWATState :uint8 {
	ESS_NORMAL,
	ESS_RILFE,

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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsAcceleration();

	bool IsHoldWeapon() { return bIsHoldWeapon; };
	
	bool IsCrouched() { return bIsCrouched; };

	ESWATState GetCurrentState() { return CurrentState; };

	class USpringArmComponent* GetCameraBoom();

protected:

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* MainCamera;

	bool bIsAcceleration = false;
	
	bool bIsHoldWeapon = false;

	bool bIsCrouched = false;

	ESWATState CurrentState = ESWATState::ESS_RILFE;

};
