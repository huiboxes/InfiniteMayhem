// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8 {
	EWS_INITIAL UMETA(DisplayName = "Initial State"),
	EWS_EQUIPPED UMETA(DisplayName = "Equipped"),
	EWS_DROP UMETA(DisplayName = "Drop"), //

	EWS_MAX UMETA(DisplayName = "DefaultMAX")

};

UCLASS()
class INFINITEMAYHEM_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere)
	EWeaponState CurrentState = EWeaponState::EWS_DROP;

protected:

	void ChangeWeaponState(EWeaponState State);

	
public:

	bool IsEquipped() { return CurrentState == EWeaponState::EWS_EQUIPPED; };

};
