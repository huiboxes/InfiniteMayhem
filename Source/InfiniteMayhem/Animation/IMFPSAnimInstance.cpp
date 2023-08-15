// Fill out your copyright notice in the Description page of Project Settings.


#include "IMFPSAnimInstance.h"
#include "../Player/SWATCharacter.h"

void UIMFPSAnimInstance::NativeUpdateAnimation(float DeltaTime) {
	Super::NativeUpdateAnimation(DeltaTime);
	if (!Player) return;

	Speed = Player->GetVelocity().Size2D();



}

void UIMFPSAnimInstance::NativeBeginPlay() {
	Super::NativeBeginPlay();
	Player = Cast<ASWATCharacter>(TryGetPawnOwner());
}
