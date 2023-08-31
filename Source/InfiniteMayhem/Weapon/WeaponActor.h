// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8 {
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Standby UMETA(DisplayName = "Stand-by"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Drop UMETA(DisplayName = "Drop"),

	EWS_MAX UMETA(DisplayName = "DefaultMAX")

};

UENUM(BlueprintType)
enum class EWeaponFireState :uint8 {
	EWS_Idle UMETA(DisplayName = "Idle"),
	EWS_Firing UMETA(DisplayName = "Firing"),
	EWS_Reload UMETA(DisplayName = "Reload"),

	EWS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class INFINITEMAYHEM_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponActor();

protected:
	virtual void BeginPlay() override;


public:	
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USphereComponent* SphereCollision;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class UArrowComponent* GunBoltArrow;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties|Fire")
	class UParticleSystem* ShellEjectionFX; // 抛壳特效
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties|Fire")
	class UParticleSystem* MuzzleFlameFX; // 开火特效

	UPROPERTY(VisibleAnywhere)
	EWeaponState CurrentState = EWeaponState::EWS_Drop;
	
	UPROPERTY(VisibleAnywhere)
	EWeaponFireState CurrentFireState = EWeaponFireState::EWS_Idle;



	UFUNCTION(BlueprintCallable)
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void Projectile(FVector TargetPos);
	void FireTheAmmon();

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class UWidgetComponent* PickupWidget;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	class UAnimMontage* ReloadMontage;
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties|Fire")
	TSubclassOf<class ABullet> BulletClass;

	FTimerHandle FireTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties|Fire")
	float FiringRate = .1f;
	

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	int32 AmmonMaxCounter; // 当前弹匣的最大容量

	UPROPERTY(EditAnywhere, Category = "Weapon Properties|Sound")
	class USoundBase* FireSound; // 开火声音

	UPROPERTY(EditAnywhere, Category = "Weapon Properties|Sound")
	class USoundBase* EmptySound; // 子弹打空声音


	int32 AmmonCurrent; // 当前弹匣剩余子弹

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float MaxShootDistance = 50000;

	void HandleFire();

	FTransform MuzzleTransform;

	
public:
	void ChangeWeaponState(EWeaponState State);
	void ChangeWeaponFireState(EWeaponFireState State);
	bool IsEquipped() { return CurrentState == EWeaponState::EWS_Equipped; };
	EWeaponState GetCurrentState() { return CurrentState; };
	EWeaponFireState GetCurrentFireState() { return CurrentFireState; };
	void ShowPickupWidget(bool bShowWidget);
	void StartFire();
	void StopFire();
	void ReloadWeapon();
	void ReloadAmmonOver();


	FORCEINLINE int32 GetAmmonMaxCounter() { return AmmonMaxCounter; };
	FORCEINLINE int32 GetAmmonCurrent() { return AmmonCurrent; };

	class USkeletalMeshComponent* GetMesh();
};
