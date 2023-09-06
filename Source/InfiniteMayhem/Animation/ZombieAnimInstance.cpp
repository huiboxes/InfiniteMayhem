// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAnimInstance.h"
#include "../Zombie/ZombieCharacter.h"

void UZombieAnimInstance::NativeUpdateAnimation(float DeltaTime) {
	Super::NativeUpdateAnimation(DeltaTime);
	if (!Zombie) return;
	Speed = Zombie->GetVelocity().Size2D();
	
	bAttack = Zombie->IsAttacking();
}

void UZombieAnimInstance::NativeBeginPlay() {
	Super::NativeBeginPlay();
	Zombie = Cast<AZombieCharacter>(TryGetPawnOwner());
}
