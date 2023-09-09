// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "../Zombie/ZombieCharacter.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include <Kismet/GameplayStatics.h>


ABullet::ABullet() {
	PrimaryActorTick.bCanEverTick = false;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SetRootComponent(SphereCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");

	ProjectileMovementComponent->InitialSpeed = 750000.f;
	ProjectileMovementComponent->MaxSpeed = 750000.f;

}

void ABullet::BeginPlay() {
	Super::BeginPlay();
	SphereCollision->OnComponentHit.AddDynamic(this, &ThisClass::OnSphereHitEvent);

}

void ABullet::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void ABullet::OnSphereHitEvent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {

	FVector HitLoc = Hit.Location;

	AZombieCharacter* Zombie = Cast<AZombieCharacter>(OtherActor);

	if (Zombie) { // 如果击中僵尸
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitZombieFX, HitLoc, Hit.Normal.Rotation());
		float DamageToApply = Hit.BoneName == TEXT("head") ? Damage : Damage * CriticalImpactCoeff; // 如果打中头部就暴击
		UGameplayStatics::ApplyDamage(Zombie, DamageToApply, nullptr, nullptr, nullptr);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitZombieSound, Hit.Location);
		OnHitZombie.Broadcast();
	} else {
		if (HitDecal) { // 在击中的地方生成贴花
			UGameplayStatics::SpawnDecalAtLocation(GetWorld(), HitDecal, FVector(7.f, 7.f, 7.f), HitLoc, Hit.Normal.Rotation(), 20.f);
		}
		HitObjectHandle(Hit);
	}

	if (OtherComp && OtherComp->IsSimulatingPhysics()) { // 如果被击中的单位开启了物理模拟，就施加冲击力
		OtherComp->AddImpulse(GetVelocity() * ImpulseCoeff);
	}
	
	Destroy();
}

void ABullet::HitObjectHandle(const FHitResult& Hit) {
	FHitResult Outhit;
	// GameTraceChannel2 是自定义的 Hit Channel
	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Hit.Location, Hit.Location, 10.f, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2), false, ActorsToIgnore, EDrawDebugTrace::None, Outhit, true)) {
		UPhysicalMaterial* mat = Outhit.PhysMaterial.Get();

		FVector HitLoc = Outhit.Location;
		FVector FxScale = FVector(.4f, .4f, .4f);
		switch (mat->SurfaceType) {
			case EPhysicalSurface::SurfaceType1: // 打中了 Metal
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitMetalFX, FTransform(Outhit.Normal.Rotation(), HitLoc, FxScale));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitMetalSound, Outhit.Location);
				break;
			case EPhysicalSurface::SurfaceType2: // 打中了 Gravel
			
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitGravelFX, FTransform(Outhit.Normal.Rotation(), HitLoc, FxScale));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitGravelSound, Outhit.Location);
				break;
			case EPhysicalSurface::SurfaceType3: // 打中了 Conocrete
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitConcreteFX, FTransform(Outhit.Normal.Rotation(), HitLoc, FxScale));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitConcreteSound, Outhit.Location);
				break;
			case EPhysicalSurface::SurfaceType4: // 打中了 Wood
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitWoodFX, FTransform(Outhit.Normal.Rotation(), HitLoc, FxScale));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitWoodSound, Outhit.Location);
				break;
			case EPhysicalSurface::SurfaceType5: // 打中了 Rock
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitRockFX, FTransform(Outhit.Normal.Rotation(), HitLoc, FxScale));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitRockSound, Outhit.Location);
				break;
			case EPhysicalSurface::SurfaceType6: // 打中了 Grass
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitGrassFX, FTransform(Outhit.Normal.Rotation(), HitLoc, FxScale));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitGrassSound, Outhit.Location);
				break;
			default: // 默认打中的沙地
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitGravelFX, FTransform(Outhit.Normal.Rotation(), HitLoc, FxScale));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitGravelSound, Outhit.Location);
				break;
		}
	}
}

