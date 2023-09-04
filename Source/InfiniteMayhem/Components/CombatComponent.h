// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INFINITEMAYHEM_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	friend class ASWATCharacter;

	void EquipWeapon(class AWeaponActor* WeaponToEquip);

	void SwitchWeapon();

	bool IsFiring();
	
	float GetFireCrosshairOffset() { return FireCrosshairOffset; };

	void ChangeEquippedWeapon();

private:
	UPROPERTY(EditAnywhere, Category = "Anim Properties")
	class UAnimMontage* EquipMontage;

	class ASWATCharacter* Player;
	class AWeaponActor* EquippedWeapon;
	class AWeaponActor* StandByWeapon;

	FTimerHandle DestroyWeaponTimerHandle;

	float FireCrosshairOffset = 0;

	void UpdateFireCrosshairOffset(float DeltaTime);


};
