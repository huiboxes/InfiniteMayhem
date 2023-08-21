// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "IMFPSHUD.generated.h"

/**
 * 
 */
UCLASS()
class INFINITEMAYHEM_API AIMFPSHUD : public AHUD
{
	GENERATED_BODY()


protected:

	virtual void DrawHUD() override;

	void DrawCrosshair();
	void DrawWeaponAmmon();
	
};
