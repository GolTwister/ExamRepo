#include "MAPEK_Subsystem.h"
#include "MAPEK_Monitor.h"
#include "MAPEK_Analyzer.h"
#include "MAPEK_Planner.h"
#include "MAPEK_Executor.h"
#include "MAPEK_Knowledge.h"
#include "MAPEK_SymptomLibrary.h"
#include "MAPEK_ActionPull.h"
#include "TimerManager.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogMAPEKSubsystem, Log, All);

void UMAPEK_Subsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	DDA_VERBOSE(TEXT("MAPEK_Subsystem Initialized"));

	Monitor = NewObject<UMAPEK_Monitor>(this);
	Analyzer = NewObject<UMAPEK_Analyzer>(this);
	Planner = NewObject<UMAPEK_Planner>(this);
	Executor = NewObject<UMAPEK_Executor>(this);
	KnowledgeBase = NewObject<UMAPEK_Knowledge>(this);
	SymptomLibrary = NewObject<UMAPEK_SymptomLibrary>(this);
	ActionPull = NewObject<UMAPEK_ActionPull>(this);
	InitializeModules();

}

void UMAPEK_Subsystem::InitializeModules() {

	if (Monitor) Monitor->Initialize(this, KnowledgeBase);
	if (Analyzer) Analyzer->Initialize(KnowledgeBase, SymptomLibrary);
	if (Planner) Planner->Initialize(KnowledgeBase, ActionPull);
	if (Executor) Executor->Initialize(KnowledgeBase);
	if (KnowledgeBase) KnowledgeBase->Initialize();
	if (SymptomLibrary) SymptomLibrary->Initialize();
	if (ActionPull) ActionPull->Initialize();
}

void UMAPEK_Subsystem::Deinitialize()
{
	StopMAPECycle();
	Super::Deinitialize();
}


void UMAPEK_Subsystem::RegisterEffector(UEffector* Effector)
{
	if (!Effector) return;
	if (!RegisteredEffectors.Contains(Effector))
	{
		RegisteredEffectors.Add(Effector);
		DDA_VERBOSE(TEXT("Registered effector: %s"), *Effector->EffectorName);
	}
}

void UMAPEK_Subsystem::UnregisterEffector(UEffector* Effector)
{
	DDA_VERBOSE(TEXT("Removed effector: %s"), *Effector->EffectorName);
	RegisteredEffectors.Remove(Effector);
}

void UMAPEK_Subsystem::SetUpdateInterval(float Seconds)
{
	DDA_VERBOSE(TEXT("Set new MAPEK update interval: %f"), Seconds);
	UpdateIntervalSeconds = FMath::Max(0.1f, Seconds);
	if (bIsCycleRunning)
	{
		StopMAPECycle();
		StartMAPECycle();
	}
}

void UMAPEK_Subsystem::StartMAPECycle()
{
	if (!GetWorld()) return;

	bIsCycleRunning = true;
	GetWorld()->GetTimerManager().SetTimer(
		MAPECycleTimerHandle,
		this,
		&UMAPEK_Subsystem::RunMAPECycle,
		UpdateIntervalSeconds,
		true
	);
	DDA_VERBOSE(TEXT("MAPEK started"));
}

void UMAPEK_Subsystem::StopMAPECycle()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(MAPECycleTimerHandle);
	}
	bIsCycleRunning = false;
	DDA_VERBOSE(TEXT("MAPEK stopped"));
}

void UMAPEK_Subsystem::RunMAPECycle()
{
	DDA_VERBOSE(TEXT("--------MAPEK new cycle beginning-----------"));
	if (!Monitor || !Analyzer || !Planner || !Executor) return;



	Monitor->Update(UpdateIntervalSeconds);

	LatestPerformanceData = PerformanceData;



	if (KnowledgeBase)
	{
		KnowledgeBase->UpdatePerformanceRatio(PerformanceData.Ratio);
	}

	DDA_LOG(TEXT("Performance Ratio: %.2f"), PerformanceData.Ratio);

	FString SymptomID = Analyzer->Analyze(PerformanceData);

	FActionPlan Action = Planner->Plan(SymptomID);

	Executor->Execute(Action, RegisteredEffectors);
	DDA_VERBOSE(TEXT("--------MAPEK cycle end-----------"));
}

float UMAPEK_Subsystem::GetOverallPerformanceRatio() const
{
	return LatestPerformanceData.Ratio;
}

void UMAPEK_Subsystem::ResetSystem()
{
	StopMAPECycle();

	

	for (UEffector* Effector : RegisteredEffectors)
	{
		if (Effector) Effector->ResetToDefault();
	}

	if (Monitor) Monitor->Reset();
	if (Analyzer) Analyzer->Reset();
	if (Planner) Planner->Reset();
	if (Executor) Executor->Reset();
	if (KnowledgeBase) KnowledgeBase->Reset();

	LatestPerformanceData = FMAPEKPerformanceData();

	StartMAPECycle();
}

void UMAPEK_Subsystem::AddSensor(UBaseSensor* Sensor) {
	Monitor->RegisterSensor(Sensor);
};


void UMAPEK_Subsystem::RemoveSensor(UBaseSensor* Sensor) {
	Monitor->UnregisterSensor(Sensor);
};

void UMAPEK_Subsystem::SetPerformanceData(FMAPEKPerformanceData Data) {
	PerformanceData = Data;
};

void UMAPEK_Subsystem::LoadSymptomLibrary(const TArray<FSymptomDefinition>& NewSymptoms) {
	SymptomLibrary->LoadSymptoms(NewSymptoms);
}

void UMAPEK_Subsystem::SetActionPull(const TArray<FActionPlan>& NewActions) {
	ActionPull->LoadActions(NewActions);
};