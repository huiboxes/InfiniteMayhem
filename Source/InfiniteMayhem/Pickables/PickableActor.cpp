// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableActor.h"
#include "../Player/SWATCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"

APickableActor::APickableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionProfileName(TEXT("Custom"));

	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	// 拾取提示 UI
	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
	PickupWidget->SetVisibility(false);
	/*SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSphereEndOverlap);*/
}

void APickableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickableActor::EnableOutlineDisplay_Implementation() {
	ItemMesh->SetRenderCustomDepth(true);
}

void APickableActor::DisableOutlineDisplay_Implementation() {
	ItemMesh->SetRenderCustomDepth(false);
}


void APickableActor::Pickup_Implementation(AActor* _Owner) { // 默认情况捡到了就销毁 Actor
	GetWorldTimerManager().SetTimer(PickupTimerHandle, this, &APickableActor::DestroySelf, 0.2f, false);
}

void APickableActor::ShowPickupWidget(bool bShowWidget) {
	if (PickupWidget) {
		PickupWidget->SetVisibility(bShowWidget);
		UUserWidget* UserWidget = PickupWidget->GetUserWidgetObject();
		if (UserWidget != nullptr) {
			UTextBlock* TextBlock = Cast<UTextBlock>(UserWidget->GetWidgetFromName(TEXT("Pickup")));
			if (TextBlock != nullptr) {
				TextBlock->SetText(FText::FromString(FString::Printf(TEXT("F 拾取 %s"), *GetItemName().ToString())));
			}
		}
	}
}

void APickableActor::DestroySelf() {
	Destroy();
}
