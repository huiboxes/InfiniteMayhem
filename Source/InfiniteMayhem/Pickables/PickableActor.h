// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/IPickableInterface.h"
#include "PickableActor.generated.h"

UCLASS()
class INFINITEMAYHEM_API APickableActor : public AActor, public IIPickableInterface
{
	GENERATED_BODY()
	
public:	
	APickableActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void EnableOutlineDisplay_Implementation() override;

	virtual void DisableOutlineDisplay_Implementation() override;

	virtual void Pickup_Implementation(AActor* _Owner) override;

	FORCEINLINE FName GetItemName() { return ItemName; };

	void ShowPickupWidget(bool bShowWidget);

protected:
	UPROPERTY(VisibleAnywhere, Category = "PickableItem Properties")
	class USceneComponent* RootComp;
	
	UPROPERTY(VisibleAnywhere, Category = "PickableItem Properties")
	class USkeletalMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, Category = "PickableItem Properties")
	class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, Category = "PickableItem Properties")
	class UWidgetComponent* PickupWidget;

	FTimerHandle PickupTimerHandle;

	UPROPERTY(EditAnywhere, Category = "PickableItem Properties")
	FName ItemName = "";

	void DestroySelf();

};
