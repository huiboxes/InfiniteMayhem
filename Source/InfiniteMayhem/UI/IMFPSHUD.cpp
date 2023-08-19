// Fill out your copyright notice in the Description page of Project Settings.


#include "IMFPSHUD.h"
#include "Engine/Canvas.h"
#include <Kismet/GameplayStatics.h>
#include "../Player/SWATCharacter.h"

void AIMFPSHUD::DrawHUD() {
	Super::DrawHUD();

	DrawCrosshair();
}

void AIMFPSHUD::DrawCrosshair() {
	ASWATCharacter* Player = Cast<ASWATCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!Player || !Player->IsHoldWeapon()) return;

	float TargetX = Canvas->ClipX / 2;
	float TargetY = Canvas->ClipY / 2;

	float LineLength = 10;
	float Offset = 5;

	// 上
	DrawLine(TargetX, TargetY - (LineLength + Offset), TargetX, TargetY - Offset, FLinearColor::Yellow);
	// 下
	DrawLine(TargetX, TargetY + Offset, TargetX, TargetY + LineLength + Offset, FLinearColor::Yellow);
	
	// 右
	DrawLine(TargetX + Offset, TargetY, TargetX + LineLength + Offset, TargetY,FLinearColor::Yellow);
	// 左
	DrawLine(TargetX - (LineLength + Offset), TargetY, TargetX - Offset, TargetY, FLinearColor::Yellow);
}
