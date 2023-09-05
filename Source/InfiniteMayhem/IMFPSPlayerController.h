// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "IMFPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEMAYHEM_API AIMFPSPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AIMFPSPlayerController();

	FGenericTeamId TeamId = FGenericTeamId(4);
	virtual FGenericTeamId GetGenericTeamId() const override;
	
};
