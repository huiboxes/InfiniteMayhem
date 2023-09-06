// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieCharacter.generated.h"


UENUM(BlueprintType)
enum class ERotationMode :uint8 {
	ERM_Yaw UMETA(DisplayName = "Yaw"),
	ERM_Orient UMETA(DisplayName = "Orient"),

	ERM_MAX
};

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
	bool bIsAttacking = false; // 是否正在攻击
	bool bHasPlayedScreamAnimation;

	bool RandomWalkHasExecuted = false; // RandomWalkTimerHandle 中的 DoOnce

	ERotationMode CurrentRotationMode = ERotationMode::ERM_Orient;
	void ChangeRotationMode(ERotationMode Mode);

	TArray<AActor*> ActorsToIgnore; // 忽略的不可受攻击者

	UPROPERTY(EditAnywhere, Category = "Zombie Properties|Anim")
	class UAnimMontage* ScreamMontage; // 尖叫声音

	UPROPERTY(EditAnywhere, Category = "Zombie Properties|Sound")
	class USoundBase* ChaseSound; // 追逐玩家的声音

public:

	void RandomWalk(); // 没发现玩家时
	void SawThePlayer(); // 发现玩家时

	FORCEINLINE bool IsAttacking() { return bIsAttacking; };

};
