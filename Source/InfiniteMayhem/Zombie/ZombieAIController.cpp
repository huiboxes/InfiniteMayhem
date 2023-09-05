// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"


AZombieAIController::AZombieAIController() {
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	
	// 设置视觉
	AISenseConfigSight = CreateDefaultSubobject<UAISenseConfig_Sight>("SenseSight");
	AISenseConfigSight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfigSight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfigSight->DetectionByAffiliation.bDetectNeutrals = false;
	AISenseConfigSight->LoseSightRadius = 2500.f;
	AISenseConfigSight->SightRadius = 1500.f;
	AISenseConfigSight->PeripheralVisionAngleDegrees = 60.f;

	// 设置听觉
	AISenseConfigHearing = CreateDefaultSubobject<UAISenseConfig_Hearing>("SenseHearing");
	AISenseConfigHearing->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfigHearing->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfigHearing->DetectionByAffiliation.bDetectNeutrals = false;
	AISenseConfigHearing->HearingRange = 3000.f;


	AIPerceptionComp->ConfigureSense(*AISenseConfigSight);
	AIPerceptionComp->ConfigureSense(*AISenseConfigHearing);
	AIPerceptionComp->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	AAIController::SetGenericTeamId(FGenericTeamId(1));

}

void AZombieAIController::BeginPlay() {
	AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnTargetPerceptionUpdated);
}


ETeamAttitude::Type AZombieAIController::GetTeamAttitudeTowards(const AActor& Other) const {

	if (APawn const* OtherPawn = Cast<APawn>(&Other)) {
		if (auto const TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController())) {
			FGenericTeamId TeamId = TeamAgent->GetGenericTeamId();
			bool isFriendlyTeam = (TeamId == FGenericTeamId(0) || TeamId == FGenericTeamId(1));
			return isFriendlyTeam ? ETeamAttitude::Friendly : ETeamAttitude::Hostile;
		}
	}
	return ETeamAttitude::Neutral;
}

void AZombieAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus) {

	if (Stimulus.IsValid()) {
		FAISenseID Sightid = AISenseConfigSight->GetSenseID();
		FAISenseID Hearid = AISenseConfigHearing->GetSenseID();

		if (Stimulus.Type == Sightid) {
			UE_LOG(LogTemp, Warning, TEXT("I See You Name By AISenseConfig_Sight::::%s"), *Actor->GetName());
		} else if (Stimulus.Type == Hearid) {
			UE_LOG(LogTemp, Warning, TEXT("I Hear You Name By AISenseConfig_Hearing::::%s"), *Actor->GetName());
		}

	}
}
