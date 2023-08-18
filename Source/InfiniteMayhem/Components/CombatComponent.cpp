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
	if (!EquippedWeapon || ((EquippedWeapon && StandByWeapon))) { // 手上未持有武器，或者手上和背包都有武器，则将武器放到手上插槽
		const USkeletalMeshSocket* RightHandSocket = Player->GetMesh()->GetSocketByName(FName("S_Rilfe"));
		if (EquippedWeapon && StandByWeapon) { // 如果手上有也背了一把枪，把手上的 Mesh 先去掉
			if (RightHandSocket) {
				EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			}
		}
		EquippedWeapon = WeaponToEquip;
		EquippedWeapon->ChangeWeaponState(EWeaponState::EWS_EQUIPPED);

		
		if (RightHandSocket) { // 将武器附加到角色 Mesh 上
			//RightHandSocket->AttachActor(EquippedWeapon, Player->GetMesh());
			EquippedWeapon->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("S_Rilfe"));
		}

		EquippedWeapon->SetOwner(Player);
		EquippedWeapon->ShowPickupWidget(false);
	} else if(EquippedWeapon && !StandByWeapon) { // 如果手上有武器，备用武器槽没有武器则放到备用插槽
		StandByWeapon = WeaponToEquip;
		StandByWeapon->ChangeWeaponState(EWeaponState::EWS_EQUIPPED);

		const USkeletalMeshSocket* SpareWeaponSocket = Player->GetMesh()->GetSocketByName(FName("S_SpareWeapon"));

		if (SpareWeaponSocket) { // 将武器附加到角色 Mesh 上
			//SpareWeaponSocket->AttachActor(StandByWeapon, Player->GetMesh());
			StandByWeapon->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("S_SpareWeapon"));

		}

		StandByWeapon->SetOwner(Player);
		StandByWeapon->ShowPickupWidget(false);
	}
	
	Player->ChangeState(ESWATState::ESS_Rilfe);
}

