// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieCharacter.generated.h"

UCLASS()
class INFINITEMAYHEM_API AZombieCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AZombieCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	FTimerHandle SawThePlayerTimerHandle;
	FTimerHandle RandomWalkTimerHandle;

	bool bAllowScream = true; // 是否可以尖叫
	bool bIsScreaming = false; // 是否可以尖叫
	bool bIsDead = false; // 是否已经死亡
	bool bHasPlayedScreamAnimation;

	bool RandomWalkHasExecuted = false; // RandomWalkTimerHandle 中的 DoOnce

	UPROPERTY(EditAnywhere, Category = "Zombie Properties|Anim")
	class UAnimMontage* ScreamMontage; // 尖叫声音

	UPROPERTY(EditAnywhere, Category = "Zombie Properties|Sound")
	class USoundBase* ChaseSound; // 追逐玩家的声音

public:

	void RandomWalk(); // 没发现玩家时
	
	void SawThePlayer(); // 发现玩家时



};