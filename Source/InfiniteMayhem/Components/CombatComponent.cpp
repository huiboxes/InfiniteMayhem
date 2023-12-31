#include "CombatComponent.h"
#include "../Player/SWATCharacter.h"
#include "../Weapon/Weapon.h"
#include "Engine/SkeletalMeshSocket.h"


UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Player = Cast<ASWATCharacter>(GetOwner());

}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateFireCrosshairOffset(DeltaTime);
}

// 偷个懒，暂时只能拾取 EWS_Initial 的武器
void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip) { 
	if (!Player || !WeaponToEquip || (WeaponToEquip->GetCurrentState() != EWeaponState::EWS_Initial)) return;

	if (!EquippedWeapon || ((EquippedWeapon && StandByWeapon))) { // 手上未持有武器，或者手上和背包都有武器，则将武器放到手上插槽
		const USkeletalMeshSocket* RightHandSocket = Player->GetMesh()->GetSocketByName(FName("S_Rilfe"));
		if (EquippedWeapon && StandByWeapon && RightHandSocket) { // 如果手上有也背了一把枪，把手上的 Mesh 先去掉

			EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			EquippedWeapon->ChangeWeaponState(EWeaponState::EWS_Drop);
			EquippedWeapon->WeaponDestroy();

			/*GetWorld()->GetTimerManager().SetTimer(DestroyWeaponTimerHandle, [=]() {
				EquippedWeapon->WeaponDestroy();
			}, 30, false);*/

			// todo 在地上生成一个新的 Pickable 的武器
		}
		EquippedWeapon = WeaponToEquip;
		EquippedWeapon->ChangeWeaponState(EWeaponState::EWS_Equipped);

		
		if (RightHandSocket) { // 将武器附加到角色 Mesh 上
			EquippedWeapon->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("S_Rilfe"));
		}

		EquippedWeapon->SetOwner(Player);
		//EquippedWeapon->ShowPickupWidget(false);
	} else if(EquippedWeapon && !StandByWeapon) { // 如果手上有武器，备用武器槽没有武器则放到备用插槽
		StandByWeapon = WeaponToEquip;
		StandByWeapon->ChangeWeaponState(EWeaponState::EWS_Equipped);

		const USkeletalMeshSocket* SpareWeaponSocket = Player->GetMesh()->GetSocketByName(FName("S_SpareWeapon"));

		if (SpareWeaponSocket) { // 将武器附加到角色 Mesh 上
			StandByWeapon->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("S_SpareWeapon"));
			EquippedWeapon->ChangeWeaponState(EWeaponState::EWS_Standby);
		}

		StandByWeapon->SetOwner(Player);
		//StandByWeapon->ShowPickupWidget(false);
	}
	
}

void UCombatComponent::SwitchWeapon() { // 只有有两把武器时才能切换武器
	if (!Player || !EquippedWeapon || !StandByWeapon) return;

	bool bWeaponIdle = Player->GetEquippedWeapon()->GetCurrentFireState() == EWeaponFireState::EWS_Idle;
	if (Player->IsEquiping() || !bWeaponIdle) return; // 如果正在换枪或者武器不处于空闲状态，则不允许切换
	Player->EnableEquiping();
	
	Player->PlayAnimMontage(EquipMontage, 1, TEXT("Default"));
}

void UCombatComponent::ChangeEquippedWeapon() {
	const USkeletalMeshSocket* RightHandSocket = Player->GetMesh()->GetSocketByName(FName("S_Rilfe"));
	const USkeletalMeshSocket* SpareWeaponSocket = Player->GetMesh()->GetSocketByName(FName("S_SpareWeapon"));

	if (!RightHandSocket || !SpareWeaponSocket) return;  // 确保获取到插槽

	EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	EquippedWeapon->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("S_SpareWeapon"));
	EquippedWeapon->ChangeWeaponState(EWeaponState::EWS_Standby);

	StandByWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	StandByWeapon->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("S_Rilfe"));
	StandByWeapon->ChangeWeaponState(EWeaponState::EWS_Equipped);

	// 更新EquippedWeapon和StandByWeapon的指针引用
	AWeapon* Temp = EquippedWeapon;
	EquippedWeapon = StandByWeapon;
	StandByWeapon = Temp;

	StandByWeapon->SetOwner(Player);
	//StandByWeapon->ShowPickupWidget(false);
	EquippedWeapon->SetOwner(Player);
	//EquippedWeapon->ShowPickupWidget(false);
}

bool UCombatComponent::IsFiring() {
	return EquippedWeapon && EquippedWeapon->GetCurrentFireState() == EWeaponFireState::EWS_Firing;
}

void UCombatComponent::UpdateFireCrosshairOffset(float DeltaTime) {
	if (!Player) return;
	float Speed = Player->GetVelocity().Size();
	float Offset = 0;


	if (Speed < 150) {
		Offset = 15;
		// FireCrosshairOffset = 5;
	} else if (Speed < 300) {
		Offset = 25;
		// FireCrosshairOffset = 15;
	} else if (Speed < 500) {
		Offset = 35;
		// FireCrosshairOffset = 25;
	} else if(Speed < 660) { 
		Offset = 50;
		//FireCrosshairOffset = 35;
	} else { // 实在太快了，不显示准星
		Offset = 100;
	}
	float RecoilOffset = Player->IsFiring() ? 20 : 0; // 开枪则增加准星偏移

	FireCrosshairOffset = FMath::FInterpTo(FireCrosshairOffset, Offset + RecoilOffset, DeltaTime, 3);
}

void UCombatComponent::EnableWeaponSimulatePhysics() {
	if (EquippedWeapon) {
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		EquippedWeapon->GetMesh()->SetSimulatePhysics(true);
		EquippedWeapon = nullptr;
	}
	if (StandByWeapon) {
		StandByWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		StandByWeapon->GetMesh()->SetSimulatePhysics(true);
		StandByWeapon = nullptr;
	}
}