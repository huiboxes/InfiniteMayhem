
#include "ZombieCharacter.h"
#include "ZombieAIController.h"
#include "../Player/SWATCharacter.h"

#include "Perception/AIPerceptionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "Components/PoseableMeshComponent.h"
#include "Kismet/GameplayStatics.h"



AZombieCharacter::AZombieCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = 40;
	/*PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("CharacterPoseableMesh"));
	PoseableMesh->SetupAttachment(RootComponent);*/
}

void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitZombie();
	
}

void AZombieCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsDead) return;
	AZombieAIController* ZombieAC = Cast<AZombieAIController>(GetController());
	if (ZombieAC) {
		bool bPlayerInSight = ZombieAC->GetBBComponent()->GetValueAsBool(TEXT("bPlayerInSight"));
		if (bPlayerInSight) {
			FHitResult Outhit;
			FVector Start = GetActorLocation();
			FVector End = GetActorForwardVector() * 110 + Start;
			if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 100.f, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel5), false, ActorsToIgnore, EDrawDebugTrace::None, Outhit, true)) {
				ASWATCharacter* Player = Cast<ASWATCharacter>(Outhit.Actor);
				bIsAttacking = (Player && !Player->IsDead());
			} else {
				bIsAttacking = false;
			}
			ChangeRotationMode(bIsAttacking ? ERotationMode::ERM_Yaw : ERotationMode::ERM_Orient);
		}
	}

}

void AZombieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AZombieCharacter::ChangeRotationMode(ERotationMode Mode) {
	switch (Mode) {
	case ERotationMode::ERM_Yaw:
		GetCharacterMovement()->bUseControllerDesiredRotation = 1; // 1 为用， 0 为不用
		break;
	case ERotationMode::ERM_Orient:
		GetCharacterMovement()->bOrientRotationToMovement = true;
		break;
	default:
		break;
	}
	CurrentRotationMode = Mode;
}

void AZombieCharacter::RandomWalk() {
	if (bIsDead) return;
	GetCharacterMovement()->MaxWalkSpeed = 40;

}

void AZombieCharacter::SawThePlayer() {
	if (bIsDead) return;
	if ((!bAllowScream && bIsScreaming)) return;
	bAllowScream = false;

	float Time = 0;
	if (!bHasPlayedScreamAnimation && (FMath::RandBool() || !ScreamMontage)) { // 部分丧尸会发出叫声，部分不会
		Time = PlayAnimMontage(ScreamMontage);
		bIsScreaming = true;
		bHasPlayedScreamAnimation = true;
	}

	GetWorld()->GetTimerManager().SetTimer(SawThePlayerTimerHandle, [=]() {
		bIsScreaming = false;
		GetCharacterMovement()->MaxWalkSpeed = FMath::RandRange(400.f, 500.f);
		UGameplayStatics::SpawnSoundAttached(ChaseSound, GetMesh(), TEXT("head")); // 播放丧尸追逐声音
	}, Time, false);
}

void AZombieCharacter::InitZombie() {
	int32 Index = FMath::RandRange(0, 11);
	GetMesh()->SetSkeletalMesh(ZombieMeshArray[Index]); // 随机外观

	SetActorRotation(FRotator(0, FMath::RandRange(-180, 180), 0)); // 随机朝向
}
