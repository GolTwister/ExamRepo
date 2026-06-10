#include "MAPEK_Monitor.h"
#include "MAPEK_Subsystem.h"
#include "MAPEK_Knowledge.h"
#include "BaseSensor.h"

void UMAPEK_Monitor::Initialize(UMAPEK_Subsystem* InSubsystem, UMAPEK_Knowledge* InKnowledge)
{
    Subsystem = InSubsystem;
    KnowledgeBase = InKnowledge;
    Reset();

    DDA_LOG(TEXT("Monitor: Monitor initialized"));
}

void UMAPEK_Monitor::Reset()
{
    TimeSinceLastCollect = 0.0f;
    for (UBaseSensor* Sensor : RegisteredSensors)
    {
        if (Sensor) Sensor->ResetSensor();
    }
}

void UMAPEK_Monitor::Update(float DeltaTime)
{
    TimeSinceLastCollect += DeltaTime;
    if (TimeSinceLastCollect >= CollectInterval)
    {
        Collect();
        DDA_LOG(TEXT("Monitor: Data collect"));
    }
}

void UMAPEK_Monitor::Collect()
{
    TimeSinceLastCollect = 0.0f;

    if (!Subsystem) return;

    FMAPEKPerformanceData PerformanceData;

    float TotalWeighted = 0.0f;
    float TotalWeight = 0.0f;

    for (UBaseSensor* Sensor : RegisteredSensors)
    {
        if (!Sensor) continue;

        float Metric = Sensor->CollectMetric();
        float Target = Sensor->GetTargetMetric();
        float Ratio = (Target > 0.0f) ? Metric / Target : 1.0f;
        float Weight = Sensor->MeasureWeight;

        PerformanceData.SensorValues.Add(Sensor->SensorName, Ratio);

        TotalWeighted += Ratio * Weight;
        TotalWeight += Weight;
    }

    float FinalRatio = (TotalWeight > 0.0f) ? TotalWeighted / TotalWeight : 1.0f;

    PerformanceData.CurrentValue = TotalWeighted;
    PerformanceData.TargetValue = TotalWeight;
    PerformanceData.Ratio = FinalRatio;

    Subsystem->SetPerformanceData(PerformanceData);
}


void UMAPEK_Monitor::RegisterSensor(UBaseSensor* Sensor)
{
    if (!Sensor) return;
    if (!RegisteredSensors.Contains(Sensor))
    {
        RegisteredSensors.Add(Sensor);

        DDA_VERBOSE(TEXT("Registered sensor: %s"), *Sensor->SensorName);
    }
}

void UMAPEK_Monitor::UnregisterSensor(UBaseSensor* Sensor)
{
    DDA_VERBOSE(TEXT("Removed sensor: %s"), *Sensor->SensorName);
    RegisteredSensors.Remove(Sensor);
}
