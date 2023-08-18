#include "CombatComponent.h"
#include "../Player/SWATCharacter.h"
#include "../Weapon/WeaponActor.h"
#include "Engine/SkeletalMeshSocket.h"


UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Player = Cast<ASWATCharacter>(GetOwner());
	// ...

}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombatComponent::EquipWeapon(AWeaponActor* WeaponToEquip) {
	if (!Player || !WeaponToEquip) return;
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->ChangeWeaponState(EWeaponState::EWS_EQUIPPED);

	const USkeletalMeshSocket* RightHandSocket = Player->GetMesh()->GetSocketByName(FName("S_Rilfe"));
	if (RightHandSocket) {
		// 将武器附加到角色 Mesh 上
		RightHandSocket->AttachActor(EquippedWeapon, Player->GetMesh());
	}
	EquippedWeapon->SetOwner(Player);
	EquippedWeapon->ShowPickupWidget(false);

	Player->ChangeState(ESWATState::ESS_Rilfe);
}

