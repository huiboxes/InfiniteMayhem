// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"

#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEMAYHEM_API AZombieAIController : public AAIController
{
	GENERATED_BODY()


public:
	AZombieAIController();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

protected:


	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAIPerceptionComponent* AIPerceptionComp = nullptr;

	class UAISenseConfig_Sight* AISenseConfigSight = nullptr;

	class UAISenseConfig_Hearing* AISenseConfigHearing = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BTree;
	

private:
	UPROPERTY()
	class UBehaviorTreeComponent* BTComponent;

	UPROPERTY()
	class UBlackboardComponent* BBComponent;
	
public:
	FORCEINLINE UBlackboardComponent* GetBBComponent() const { return BBComponent; }
	FORCEINLINE UBehaviorTreeComponent* GetBTComponent() const { return BTComponent; }

};
