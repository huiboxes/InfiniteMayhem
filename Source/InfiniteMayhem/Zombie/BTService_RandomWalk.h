// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_RandomWalk.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEMAYHEM_API UBTService_RandomWalk : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_RandomWalk();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	float RandomRadius;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector TargetLocationKey;
};
