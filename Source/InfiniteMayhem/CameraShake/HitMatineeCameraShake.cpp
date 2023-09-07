// Fill out your copyright notice in the Description page of Project Settings.


#include "HitMatineeCameraShake.h"

UHitMatineeCameraShake::UHitMatineeCameraShake() {
    OscillationDuration = 0.25f;
    OscillationBlendInTime = 0.08f;
    OscillationBlendOutTime = 0.2f;

    RotOscillation.Pitch.Amplitude = FMath::RandRange(1.0f, 3.0f);
    RotOscillation.Pitch.Frequency = FMath::RandRange(2.0f, 4.0f);

    RotOscillation.Yaw.Amplitude = FMath::RandRange(1.0f, 3.0f);
    RotOscillation.Yaw.Frequency = FMath::RandRange(2.0f, 4.0f);
}
