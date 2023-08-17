// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "IMFPSAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEMAYHEM_API UIMFPSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeUpdateAnimation(float DeltaTime) override;
	virtual void NativeBeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	float Speed;
	
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsCrouched;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsInAir;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsIronsight;
	
	UPROPERTY(BlueprintReadOnly)
	float Direction;

	UPROPERTY(BlueprintReadOnly)
	float AO_Yaw;

	UPROPERTY(BlueprintReadOnly)
	float AO_Pitch;

	UPROPERTY()
	class ASWATCharacter* Player;

	
	
};
