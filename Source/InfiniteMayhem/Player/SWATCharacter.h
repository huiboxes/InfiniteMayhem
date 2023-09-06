// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SWATCharacter.generated.h"

UENUM(BlueprintType)
enum class ESWATState :uint8 {
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_Rilfe UMETA(DisplayName = "Rilfe"),

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
	void IronsightButtonPressed();
	void IronsightButtonReleased();
	void PickupButtonPressed();
	void SwitchWeaponButtonPressed();
	void SwitchFireModeButtonPressed();
	void ToggleFire();
	void StopFire();
	void ReloadWeaponButtonPressed();
	bool PickRangeDetection(FHitResult& Hit);
	bool OutlineDisplayDetection();

	void UpdateCameraTargetPos(float DeltaTime);
	void AimOffset(float DeltaTime);

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsAcceleration();
	bool IsHoldWeapon();
	bool IsInAir();
	bool IsAiming();
	bool IsFiring();

	void EnableEquiping() ;
	void ChangeState(ESWATState State);


	class USpringArmComponent* GetCameraBoom();
	void SetOverlappingWeapon(class AWeaponActor* Weapon);
	class AWeaponActor* GetEquippedWeapon();

	void FootstepHandle(FVector ToeLoc); // 传入的脚趾位置，判断踩到的什么材质
	void PlayLeftFootStep(); // 播放左脚脚步
	void PlayRighttFootStep(); // 播放右脚脚步

	FORCEINLINE	bool IsCrouched() { return bIsCrouched; };
	FORCEINLINE void DisableEquiping() { bIsEquiping = false; };
	FORCEINLINE bool IsEquiping() { return bIsEquiping; };
	FORCEINLINE bool IsPicking() { return bPicking; };
	FORCEINLINE void CancelPicking() { bPicking = false; };
	FORCEINLINE bool IsDead() { return bIsDead; };
	FORCEINLINE void SetDead(bool _Dead) { bIsDead = _Dead; };
	FORCEINLINE ESWATState GetCurrentState() { return CurrentState; };
	FORCEINLINE class UCombatComponent* GetCombatComp() { return CombatComp; };
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; };
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; };
	FORCEINLINE float GetSpeed() const { return Speed; };
	FORCEINLINE float GetStanding() { return Standing; };
	FORCEINLINE void SetStanding(float _Standing) { Standing = _Standing; };


protected:

	UPROPERTY(VisibleAnywhere, Category = "Character Properties|AI")
	class UPawnNoiseEmitterComponent* PawnNoiseEmitterComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Character Properties")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Character Properties")
	class UCameraComponent* MainCamera;
	
	UPROPERTY(VisibleAnywhere, Category = "Character Properties")
	class AWeaponActor* OverlappingWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Character Properties")
	class UCombatComponent* CombatComp;

	UPROPERTY(EditAnywhere, Category = "Character|Sound")
	class USoundBase* RaiseArmSound; // 子弹在地上反复弹跳的声音
	
	UPROPERTY(EditAnywhere, Category = "Character|Sound")
	class USoundBase* HolsterSound; // 子弹在地上反复弹跳的声音

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Sound|Step Sound|Default")
	class USoundBase* HitDefaultSound; // 默认踩到地上的音效

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Sound|Step Sound|Metal")
	class USoundBase* OnMetalSound; // 踩到金属后的音效

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Sound|Step Sound|Gravel")
	class USoundBase* OnGravelSound; // 踩到沙地后的音效
		   
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Sound|Step Sound|Concrete")
	class USoundBase* OnConcreteSound; // 踩到混凝土后的音效

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Sound|Step Sound|Wood")
	class USoundBase* OnWoodSound; // 踩到木头后的音效
		  
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Sound|Step Sound|Rock")
	class USoundBase* OnRockSound; // 踩到岩石后的音效
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Sound|Step Sound|Grass")
	class USoundBase* OnGrassSound; // 踩到草地后的音效

	bool bIsAcceleration = false;
	bool bIsCrouched = false;
	bool bAiming = false;
	bool bIsEquiping = false;
	bool bPicking = false;
	bool bIsDead = false;

	FTimerHandle PickupTimerHandle;
	float Standing = 1.f;

	ESWATState CurrentState = ESWATState::ESS_Normal;

	float CameraBoomSocketYOffset = 20;
	float CameraBoomSocketZOffset = 40;
	float CameraXOffset = 0;
	

	// 驱动 AimOffset 的 Yaw
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float AO_Yaw;

	// 驱动 AimOffset 的 Pitch
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float AO_Pitch = 0;

	float Speed = 0;

private:
	TArray<AActor*> ActorsToIgnoreInFootstep; // 脚步校验函数检测时忽略的Actor

	TArray<AActor*> ActorsToIgnoreInPickRange; // 拾取检测函数忽略的Actor

	AActor* DetectedPickableItem;
	
	bool bOutlineDisplayDetectionReset = true;
};
