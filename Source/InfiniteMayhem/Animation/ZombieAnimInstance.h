// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ZombieAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEMAYHEM_API UZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	virtual void NativeBeginPlay() override;

protected:
	FTimerHandle AttackTimerHandle;

	void AttackDetection();

	UPROPERTY()
	class AZombieCharacter* Zombie;

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float Speed;
	
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bAttack = false; // 是否在攻击别人

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bBeAttacked = false ; // 是否被击中


	UFUNCTION()
	void AnimNotify_AttactStart();
	
	UFUNCTION()
	void AnimNotify_AttactEnd();
};
