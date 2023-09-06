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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	void InitZombie();

	UPROPERTY(EditAnywhere, Category = "Zombie Properties|State")
	float Health = 100; // 生命值

	FTimerHandle SawThePlayerTimerHandle;
	FTimerHandle RandomWalkTimerHandle;

	bool bAllowScream = true; // 是否可以尖叫
	bool bIsScreaming = false; // 是否可以尖叫
	bool bIsAttacking = false; // 是否正在攻击
	bool bBeAttacked = false; // 是否正被攻击
	bool bHasPlayedScreamAnimation;

	bool RandomWalkHasExecuted = false; // RandomWalkTimerHandle 中的 DoOnce

	ERotationMode CurrentRotationMode = ERotationMode::ERM_Orient;
	void ChangeRotationMode(ERotationMode Mode);

	TArray<AActor*> ActorsToIgnore; // 忽略的不可受攻击者

	UPROPERTY(EditAnywhere, Category = "Zombie Properties|Anim")
	class UAnimMontage* ScreamMontage; // 尖叫声音

	UPROPERTY(EditAnywhere, Category = "Zombie Properties|Sound")
	class USoundBase* ChaseSound; // 追逐玩家的声音

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zombie Properties|Mesh")
	TArray<class USkeletalMesh*> ZombieMeshArray;

	class ASWATCharacter* LastAttackedPlayer = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie Properties|FX")
	class UNiagaraSystem* HitPlayerFX; // 丧尸攻击到玩家时播放的特效

	void Die();
public:

	void RandomWalk(); // 没发现玩家时
	void SawThePlayer(); // 发现玩家时
	void AttackDetection(FName StartBoneName, FName EndBoneName); // 攻击范围检测
	

	FORCEINLINE bool IsAttacking() { return bIsAttacking; };

	FORCEINLINE bool IsDead() { return Health <= 0; };
	FORCEINLINE bool GetBeAttacked() { return bBeAttacked; };
	FORCEINLINE void SetAttackingState(bool _Attacking) { bIsAttacking = _Attacking; };
	FORCEINLINE bool GetAttackingState() { return bIsAttacking; };
	FORCEINLINE void ClearAttackedPlayer() { LastAttackedPlayer = nullptr; };
};
