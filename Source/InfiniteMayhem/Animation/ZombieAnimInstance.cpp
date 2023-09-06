// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAnimInstance.h"
#include "../Zombie/ZombieCharacter.h"

void UZombieAnimInstance::NativeUpdateAnimation(float DeltaTime) {
	Super::NativeUpdateAnimation(DeltaTime);
	if (!Zombie) return;
	Speed = Zombie->GetVelocity().Size2D();
	
	bAttack = Zombie->IsAttacking(); // 是否正在攻击
	bBeAttacked = Zombie->GetBeAttacked(); // 是否被攻击
}

void UZombieAnimInstance::NativeBeginPlay() {
	Super::NativeBeginPlay();
	Zombie = Cast<AZombieCharacter>(TryGetPawnOwner());
}
