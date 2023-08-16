// Fill out your copyright notice in the Description page of Project Settings.


#include "IMFPSHUD.h"
#include "Engine/Canvas.h"

void AIMFPSHUD::DrawHUD() {
	Super::DrawHUD();
	DrawCrosshair();
}

void AIMFPSHUD::DrawCrosshair() {
	float TargetX = Canvas->ClipX / 2;
	float TargetY = Canvas->ClipY / 2;

	float LineLength = 10;
	float Offset = 5;

	DrawLine(TargetX, TargetY - (LineLength + Offset), TargetX, TargetY - Offset, FLinearColor::Yellow);
	DrawLine(TargetX, TargetY + Offset, TargetX, TargetY + LineLength + Offset, FLinearColor::Yellow);
	
	DrawLine(TargetX + Offset, TargetY, TargetX + LineLength + Offset, TargetY,FLinearColor::Yellow);
	DrawLine(TargetX - (LineLength + Offset), TargetY, TargetX - Offset, TargetY, FLinearColor::Yellow);
}
