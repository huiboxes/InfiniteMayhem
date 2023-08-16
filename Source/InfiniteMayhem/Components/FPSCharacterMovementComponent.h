// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FPSCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEMAYHEM_API UFPSCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	virtual float GetMaxSpeed() const override;

protected:

	UPROPERTY(EditAnywhere, Category = "UFPSCharacterMoveMent")
	float CrouchMaxWalkSpeed = 300;

	UPROPERTY(EditAnywhere, Category = "UFPSCharacterMoveMent")
	float CrouchMaxRunSpeed = 500;

	UPROPERTY(EditAnywhere, Category = "UFPSCharacterMoveMent")
	float NormalMaxWalkSpeed = 600;

	UPROPERTY(EditAnywhere, Category = "UFPSCharacterMoveMent")
	float NormalMaxRunSpeed = 900;

	UPROPERTY(EditAnywhere, Category = "UFPSCharacterMoveMent")
	float RilfeMaxWalkSpeed = 650;

	UPROPERTY(EditAnywhere, Category = "UFPSCharacterMoveMent")
	float RilfeMaxRunSpeed = 1000;

protected:

	float GetPlayerCurrentSpeed() const;
	
	
};
