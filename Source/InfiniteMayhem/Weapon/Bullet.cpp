// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include <Kismet/GameplayStatics.h>


ABullet::ABullet() {
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SetRootComponent(SphereCollision);


	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->bAutoActivate = false; // 这样发射物不会自动开始移动
	ProjectileMovementComponent->UpdatedComponent = RootComponent; // 设置更新的组件

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
	UGameplayStatics::SpawnDecalAtLocation();


}

