// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSensor.h"
#include "MAPEK_Monitor.h"

// Sets default values for this component's properties
UBaseSensor::UBaseSensor()
{
    SensorName = TEXT("BaseSensor");
    MeasureWeight = 1.0f;
}

float UBaseSensor::CollectMetric_Implementation()
{
    if (CollectWithUpdate)
        MeasureMetric(0.0f);
    DDA_VERBOSE(TEXT("Sensor: %s collected"), *SensorName);
    return CurrentMetric;
}

float UBaseSensor::GetTargetMetric_Implementation()
{
    return 1.0f;
}

void UBaseSensor::ResetSensor_Implementation()
{
    CurrentMetric = 0.0f;
}

void UBaseSensor::MeasureMetric(float DeltaTime)
{
    OnMeasureMetric.Broadcast(DeltaTime);
    DDA_VERBOSE( TEXT("Sensor: %s measured"), *SensorName);
};

void UBaseSensor::BeginPlay() {
    Super::BeginPlay();

    UGameInstance* GameInstance = GetWorld()->GetGameInstance();
    if (GameInstance)
    {
        UMAPEK_Subsystem* Subsystem = GameInstance->GetSubsystem<UMAPEK_Subsystem>();
        if (Subsystem)
        {
            Subsystem->AddSensor(this);
        }
    }
};

void UBaseSensor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    UGameInstance* GameInstance = GetWorld()->GetGameInstance();
    if (GameInstance)
    {
        UMAPEK_Subsystem* Subsystem = GameInstance->GetSubsystem<UMAPEK_Subsystem>();
        if (Subsystem)
        {
            Subsystem->RemoveSensor(this);
        }
    }

    Super::EndPlay(EndPlayReason);
};
