// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableWeapon.h"
#include "../Player/SWATCharacter.h"
#include "../Components/CombatComponent.h"
#include "../Weapon/WeaponActor.h"
#include "../Weapon/Magazine.h"


void APickableWeapon::Pickup_Implementation(AActor* _Owner) {
	Super::Pickup_Implementation(_Owner); // 销毁 Actor

	PickupTimerDelegate.BindLambda([=]() {
		AttachWeponToPlayer(Cast<ASWATCharacter>(_Owner));
	});

	GetWorldTimerManager().SetTimer(PickupTimerHandle, PickupTimerDelegate, 0.4f, false);

}

// 生成武器类并附加到手部插槽
void APickableWeapon::AttachWeponToPlayer(ASWATCharacter* Player) {
	if (Player && WeaponClass) {
		AWeaponActor* Weapon = GetWorld()->SpawnActor<AWeaponActor>(WeaponClass);
		Player->GetCombatComp()->EquipWeapon(Weapon);
	}

}
