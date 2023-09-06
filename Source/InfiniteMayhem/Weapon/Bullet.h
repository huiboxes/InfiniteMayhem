#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class INFINITEMAYHEM_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties")
	class UMaterialInterface* HitDecal; // 击中物体后的贴花

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties|Numerical")
	float Damage = 50; // 基础伤害
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties|Numerical")
	float CriticalImpactCoeff = 1.5; // 暴击系数

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties|Numerical")
	float ImpulseCoeff = 2; // 打中物体冲击力系数

	TArray<AActor*> ActorsToIgnore; // 不击中的物体

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties|Zombie")
	class UNiagaraSystem* HitZombieFX; // 击中丧尸播放的特效
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties|Zombie")
	class USoundBase* HitZombieSound; // 击中丧尸的音效

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties|Default")
	class UParticleSystem* HitDefaultFX; // 默认击中物体后的特效
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties|Default")
	class USoundBase* HitDefaultSound; // 默认击中物体后的音效

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties|Metal")
	class UParticleSystem* HitMetalFX; // 击中金属后的特效
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties|Metal")
	class USoundBase* HitMetalSound; // 击中金属后的音效

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties|Gravel")
	class UParticleSystem* HitGravelFX; // 击中沙地后的特效				   
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties|Gravel")
	class USoundBase* HitGravelSound; // 击中沙地后的音效

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties|Concrete")
	class UParticleSystem* HitConcreteFX; // 击中混凝土后的特效			   
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties|Concrete")
	class USoundBase* HitConcreteSound; // 击中混凝土后的音效

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties|Wood")
	class UParticleSystem* HitWoodFX; // 击中木头后的特效				  
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties|Wood")
	class USoundBase* HitWoodSound; // 击中木头后的音效

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties|Rock")
	class UParticleSystem* HitRockFX; // 击中岩石后的特效				  
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties|Rock")
	class USoundBase* HitRockSound; // 击中岩石后的音效

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties|Grass")
	class UParticleSystem* HitGrassFX; // 击中草地后的特效		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Properties|Grass")
	class USoundBase* HitGrassSound; // 击中草地后的音效

protected:

	UFUNCTION()
	void OnSphereHitEvent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void HitObjectHandle(const FHitResult& Hit);
};
