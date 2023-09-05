
#include "ZombieCharacter.h"
#include "Perception/AIPerceptionComponent.h"

AZombieCharacter::AZombieCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
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

