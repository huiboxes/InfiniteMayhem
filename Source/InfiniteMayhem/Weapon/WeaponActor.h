// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHandleFireDelegate);

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

	UFUNCTION()
	void OnEmptyShellCollide(FName EventName, float EmitterTime, int32 ParticleTime, FVector Location, FVector Velocity, FVector Direction, FVector Normal, FName BoneName, UPhysicalMaterial* PhysMat);

	void Projectile(FVector TargetPos);
	void FireTheAmmon();

	UPROPERTY(BlueprintAssignable, Category = "Weapon Properties|Fire")
	FHandleFireDelegate OnWeaponFire;


private:


	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class UWidgetComponent* PickupWidget;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	class UAnimMontage* ReloadMontage;

	FTimerHandle FireTimerHandle; // 开火定时器句柄
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties|Fire")
	TSubclassOf<class ABullet> BulletClass;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties|Fire")
	UAnimationAsset* FireAnim;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties|Fire")
	float FiringRate = .1f;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties|Fire")
	bool bIsFullyAutomatic = true; // 是否全自动开火模式

	UPROPERTY(EditAnywhere, Category = "Weapon Properties|Sound")
	class USoundBase* FireSound; // 开火声音

	UPROPERTY(EditAnywhere, Category = "Weapon Properties|Sound")
	class USoundBase* EmptySound; // 子弹打空声音
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties|Sound")
	class USoundBase* ShellBounceSound; // 子弹在地上反复弹跳的声音
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties|Sound")
	class USoundBase* ShellCollideSound; // 子弹落地声音

	UPROPERTY(EditAnywhere, Category = "Weapon Properties|Ammon")
	int32 AmmonMaxCounter = 30; // 当前弹匣的最大容量

	UPROPERTY(EditAnywhere, Category = "Weapon Properties|Ammon")
	int32 MagNum = 1; // 弹夹数量

	int32 AmmonCurrent = 0; // 当前弹匣剩余子弹

	

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float MaxShootDistance = 50000;

	void HandleFire();

	FTransform MuzzleTransform; // 枪口变换信息

	bool bCanFire = true;
	void SetCanFire(bool _bCanFire) { bCanFire = _bCanFire; };

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
	bool CanFire() { return bCanFire; };


	FORCEINLINE int32 GetAmmonMaxCounter() { return AmmonMaxCounter; };
	FORCEINLINE int32 GetAmmonCurrent() { return AmmonCurrent; };
	FORCEINLINE int32 GetMagNum() { return MagNum; };
	FORCEINLINE void SwitchFireMode() { bIsFullyAutomatic = !bIsFullyAutomatic; };


	class USkeletalMeshComponent* GetMesh();
};
