// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../Player/SWATCharacter.h"

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

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float Speed;
	
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bIsCrouched;
	
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;
	
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bHoldWeapon;
	
	
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bFire;
	
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bEquiping;
	
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bPicking;
	
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float Standing;
	
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float Equiping;
	
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float AO_Yaw;

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float AO_Pitch;

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	FTransform LeftHandTransform;

	UPROPERTY()
	class ASWATCharacter* Player;


	


protected:
	void UpdateStatusFromPlayer();

	void UpdateLeftHandTransform(); // 更新手掌托扶的位置

	UFUNCTION()
	void AnimNotify_ReloadOver();
	
	UFUNCTION()
	void AnimNotify_PullOutWeapon();
	
	UFUNCTION()
	void AnimNotify_SwitchWeaponOver();

	UFUNCTION()
	void AnimNotify_RemoveMag();

	UFUNCTION()
	void AnimNotify_GenerateMag();

	UFUNCTION()
	void AnimNotify_LeftFootStep(); // 左脚脚步声
	
	UFUNCTION()
	void AnimNotify_RightFootStep(); // 右脚脚步

};
