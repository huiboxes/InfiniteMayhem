// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_RandomWalk.h"
#include "../Zombie/ZombieAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTService_RandomWalk::UBTService_RandomWalk() {
	bCreateNodeInstance = true;
}

void UBTService_RandomWalk::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // 随机生成AI新的位置
    FVector RandomLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation() + FVector(FMath::FRandRange(-1, 1), FMath::FRandRange(-1, 1), 0) * RandomRadius;

    // 将新位置设置为AI的目标位置，并激活导航
    OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationKey.SelectedKeyName, RandomLocation);
	
}
