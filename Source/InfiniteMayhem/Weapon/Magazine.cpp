// Fill out your copyright notice in the Description page of Project Settings.


#include "Magazine.h"
//#include "SkeletalMesh.h"

AMagazine::AMagazine()
{
	PrimaryActorTick.bCanEverTick = false;

	MagMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MagMesh"));
	SetRootComponent(MagMesh);

	MagMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MagMesh->SetCollisionProfileName(TEXT("PhysicsBody"));
	MagMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

}

void AMagazine::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMagazine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

