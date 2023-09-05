
#include "ZombieCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AZombieCharacter::AZombieCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->MaxWalkSpeed = 40;
}

void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZombieCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZombieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AZombieCharacter::RandomWalk() {
	GetCharacterMovement()->MaxWalkSpeed = 40;
}

void AZombieCharacter::SawThePlayer() {
	GetCharacterMovement()->MaxWalkSpeed = FMath::RandRange(400.f, 500.f);
}

