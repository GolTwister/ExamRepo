// Fill out your copyright notice in the Description page of Project Settings.


#include "Effector.h"
#include "TimerManager.h"
#include "MAPEK_Subsystem.h"
#include "Engine/World.h"

UEffector::UEffector()
{
    EffectorName = TEXT("BaseEffector");
}

void UEffector::BeginPlay() {
    Super::BeginPlay();

    UGameInstance* GameInstance = GetWorld()->GetGameInstance();
    if (GameInstance)
    {
        UMAPEK_Subsystem* Subsystem = GameInstance->GetSubsystem<UMAPEK_Subsystem>();
        if (Subsystem)
        {
            Subsystem->RegisterEffector(this);
        }
    }
};

void UEffector::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    UGameInstance* GameInstance = GetWorld()->GetGameInstance();
    if (GameInstance)
    {
        UMAPEK_Subsystem* Subsystem = GameInstance->GetSubsystem<UMAPEK_Subsystem>();
        if (Subsystem)
        {
            Subsystem->UnregisterEffector(this);
        }
    }

    DDA_VERBOSE(TEXT("Effector %s applied interped"), *EffectorName);
    Super::EndPlay(EndPlayReason);
};

void UEffector::ApplyModifier(float Value)
{
    CurrentValue = Value;
    OnApplyModifier.Broadcast(Value);
    DDA_VERBOSE(TEXT("Effector %s applied"), *EffectorName);
}

void UEffector::ApplyModifierInterp(float TargetValue, float Duration)
{
    TargetValueInterp = TargetValue;
    InterpDuration = Duration;
    InterpTime = 0.0f;

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(InterpTimerHandle, [this]()
            {
            }, 0.016f, true);
    }
    OnApplyModifierInterp.Broadcast(TargetValue, Duration);
}

void UEffector::ResetToDefault_Implementation()
{
    CurrentValue = DefaultValue;
    TargetValueInterp = DefaultValue;

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(InterpTimerHandle);
    }
}

