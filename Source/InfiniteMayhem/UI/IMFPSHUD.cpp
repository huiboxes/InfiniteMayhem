// Fill out your copyright notice in the Description page of Project Settings.


#include "IMFPSHUD.h"
#include "Engine/Canvas.h"
#include <Kismet/GameplayStatics.h>
#include "../Player/SWATCharacter.h"
#include "../Weapon/WeaponActor.h"
#include "../Components/CombatComponent.h"

void AIMFPSHUD::DrawHUD() {
	Super::DrawHUD();

	DrawWeaponAmmon();
	DrawCrosshair();

}

void AIMFPSHUD::DrawCrosshair() {
	float TargetX = Canvas->ClipX / 2.f;
	float TargetY = Canvas->ClipY / 2.f;

	// 绘制中心点
	DrawRect(FLinearColor::Yellow, TargetX - 1.f, TargetY - 1.f, 2.f, 2.f);

	ASWATCharacter* Player = Cast<ASWATCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Player && Player->IsHoldWeapon()) { // 如果玩家持有枪械时，绘制十字准星
		UCombatComponent* CombatComp =  Player->GetCombatComp();
		if (!CombatComp) return;
		float FireCrosshairOffset = CombatComp->GetFireCrosshairOffset();
		if (FireCrosshairOffset >= 80) return; // 如果偏移过大，直接不绘制准星
		if (Player->IsAiming()) FireCrosshairOffset -= 10; // 瞄准状态降低准星偏移
		float LineLength = 10.f;
		
		float Offset = 5.f + FireCrosshairOffset; 

		// 上
		DrawLine(TargetX, TargetY - (LineLength + Offset), TargetX, TargetY - Offset, FLinearColor::Yellow);
		// 下
		DrawLine(TargetX, TargetY + Offset, TargetX, TargetY + LineLength + Offset, FLinearColor::Yellow);

		// 右
		DrawLine(TargetX + Offset, TargetY, TargetX + LineLength + Offset, TargetY, FLinearColor::Yellow);
		// 左
		DrawLine(TargetX - (LineLength + Offset), TargetY, TargetX - Offset, TargetY, FLinearColor::Yellow);
	}

	
}

void AIMFPSHUD::DrawWeaponAmmon() {
	ASWATCharacter* Player = Cast<ASWATCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	float PaintX = Canvas->ClipX - 40;
	float PaintY = Canvas->ClipY - 50;

	AWeaponActor*  EquippedWeapon = Player->GetEquippedWeapon();
	if (!Player || !Player->IsHoldWeapon() || !EquippedWeapon) {
		DrawText(TEXT("--/--"),FLinearColor::Yellow, PaintX, PaintY);
		return;
	}

	FString Msg = FString::Printf(TEXT("%d/%d"), EquippedWeapon->GetAmmonCurrent(), EquippedWeapon->GetMagNum() * EquippedWeapon->GetAmmonMaxCounter());
	DrawText(Msg, FLinearColor::Yellow, PaintX, PaintY);
}

