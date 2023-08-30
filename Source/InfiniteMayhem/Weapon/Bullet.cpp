// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
//#include "Materials/MaterialInterface.h"
#include <Kismet/GameplayStatics.h>


ABullet::ABullet() {
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SetRootComponent(SphereCollision);
	SphereCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	//SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	//ProjectileMovementComponent->bAutoActivate = true; // 这样发射物不会自动开始移动
	//ProjectileMovementComponent->UpdatedComponent = RootComponent; // 设置更新的组件

	ProjectileMovementComponent->InitialSpeed = 750000.f;
	ProjectileMovementComponent->MaxSpeed = 750000.f;

}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	SphereCollision->OnComponentHit.AddDynamic(this, &ThisClass::OnSphereHitEvent);

}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::OnSphereHitEvent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	
	FVector HitLoc = Hit.Location;
	FRotator HitRot = Hit.GetActor()->GetActorRotation();
	
	if (HitDecal) { // 在击中的地方生成贴花
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), HitDecal, FVector(7.f, 7.f, 7.f), HitLoc, Hit.Normal.Rotation(), 20.f);
	}

	Destroy();
}

void ABullet::HitObjectHandle(const FHitResult& Hit) {
	

}

