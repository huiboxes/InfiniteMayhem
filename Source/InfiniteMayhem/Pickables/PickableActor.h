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

	UFUNCTION(BlueprintCallable)
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:	
	virtual void Tick(float DeltaTime) override;

	virtual void EnableOutlineDisplay_Implementation() override;

	virtual void DisableOutlineDisplay_Implementation() override;

	virtual void Pickup_Implementation(AActor* _Owner) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "PickableItem Properties")
	class USceneComponent* RootComp;
	
	UPROPERTY(VisibleAnywhere, Category = "PickableItem Properties")
	class USkeletalMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, Category = "PickableItem Properties")
	class USphereComponent* SphereCollision;

	FTimerHandle PickupTimerHandle;

	void DestroySelf();

};
