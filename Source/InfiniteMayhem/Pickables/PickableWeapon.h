// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickableActor.h"
#include "../Interface/IPickableInterface.h"

#include "PickableWeapon.generated.h"


DECLARE_DELEGATE_OneParam(FPickupTimerDelegate, ASWATCharacter*);
/**
 * 
 */
UCLASS()
class INFINITEMAYHEM_API APickableWeapon : public APickableActor
{
	GENERATED_BODY()
	
public:
	virtual void Pickup_Implementation(AActor* _Owner) override;

protected:
	FTimerHandle PickupTimerHandle;

	UPROPERTY(EditAnywhere, Category = "PickableItem Properties")
	TSubclassOf<class AWeaponActor> WeaponClass; // 生成的武器类

	UFUNCTION()
	void AttachWeponToPlayer(class ASWATCharacter* Player);

	FTimerDelegate PickupTimerDelegate;
};
