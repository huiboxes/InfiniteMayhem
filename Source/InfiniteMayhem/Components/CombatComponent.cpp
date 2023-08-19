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
				EquippedWeapon->ChangeWeaponState(EWeaponState::EWS_DROP);
			}
		}
		EquippedWeapon = WeaponToEquip;
		EquippedWeapon->ChangeWeaponState(EWeaponState::EWS_EQUIPPED);

		
		if (RightHandSocket) { // 将武器附加到角色 Mesh 上
			EquippedWeapon->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("S_Rilfe"));
		}

		EquippedWeapon->SetOwner(Player);
		EquippedWeapon->ShowPickupWidget(false);
	} else if(EquippedWeapon && !StandByWeapon) { // 如果手上有武器，备用武器槽没有武器则放到备用插槽
		StandByWeapon = WeaponToEquip;
		StandByWeapon->ChangeWeaponState(EWeaponState::EWS_EQUIPPED);

		const USkeletalMeshSocket* SpareWeaponSocket = Player->GetMesh()->GetSocketByName(FName("S_SpareWeapon"));

		if (SpareWeaponSocket) { // 将武器附加到角色 Mesh 上
			StandByWeapon->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("S_SpareWeapon"));
			EquippedWeapon->ChangeWeaponState(EWeaponState::EWS_STANDBY);
		}

		StandByWeapon->SetOwner(Player);
		StandByWeapon->ShowPickupWidget(false);
	}
	
	Player->ChangeState(ESWATState::ESS_Rilfe);
}

void UCombatComponent::SwitchWeapon() { // 只有有两把武器时才能切换武器
	if (!Player || !EquippedWeapon || !StandByWeapon) return;

	const USkeletalMeshSocket* RightHandSocket = Player->GetMesh()->GetSocketByName(FName("S_Rilfe"));
	const USkeletalMeshSocket* SpareWeaponSocket = Player->GetMesh()->GetSocketByName(FName("S_SpareWeapon"));

	if (!RightHandSocket || !SpareWeaponSocket) return;  // 确保获取到插槽

	EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	EquippedWeapon->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("S_SpareWeapon"));
	EquippedWeapon->ChangeWeaponState(EWeaponState::EWS_STANDBY);

	StandByWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	StandByWeapon->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("S_Rilfe"));
	StandByWeapon->ChangeWeaponState(EWeaponState::EWS_EQUIPPED);

	// 更新EquippedWeapon和StandByWeapon的指针引用
	AWeaponActor* Temp = EquippedWeapon;
	EquippedWeapon = StandByWeapon;
	StandByWeapon = Temp;

	StandByWeapon->SetOwner(Player);
	StandByWeapon->ShowPickupWidget(false);
	EquippedWeapon->SetOwner(Player);
	EquippedWeapon->ShowPickupWidget(false);
}
