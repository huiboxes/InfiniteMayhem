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

	UPROPERTY(VisibleAnywhere)
	EWeaponState CurrentState = EWeaponState::EWS_Drop;

	UFUNCTION(BlueprintCallable)
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class UWidgetComponent* PickupWidget;

	
public:
	void ChangeWeaponState(EWeaponState State);
	bool IsEquipped() { return CurrentState == EWeaponState::EWS_Equipped; };
	EWeaponState GetCurrentState() { return CurrentState; };
	void ShowPickupWidget(bool bShowWidget);
	void StartFire();
	void StopFire();
};
