#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Effector.h"
#include "Logger.h"
#include "MAPEK_Subsystem.generated.h"

USTRUCT(BlueprintType)
struct FMAPEKPerformanceData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	float CurrentValue = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float TargetValue = 1.0f;

	UPROPERTY(BlueprintReadOnly)
	float Ratio = 1.0f;

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, float> SensorValues;

	auto GetSensorValue(FString SensorName) {
		return SensorValues.Find(SensorName);
	};
};

USTRUCT(BlueprintType)
struct FSymptomDefinition : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SymptomID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinPerformanceRatio = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxPerformanceRatio = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Priority = 0;
};

class UMAPEK_Monitor;
class UMAPEK_Analyzer;
class UMAPEK_Planner;
class UMAPEK_Executor;
class UMAPEK_Knowledge;
class UMAPEK_SymptomLibrary;
class UMAPEK_ActionPull;
class UBaseSensor;

UCLASS()
class DDATYPED_API UMAPEK_Subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void InitializeModules();
	virtual void Deinitialize() override;


	UFUNCTION(BlueprintCallable, Category = "MAPEKit")
	void RegisterEffector(UEffector* Effector);

	UFUNCTION(BlueprintCallable, Category = "MAPEKit")
	void UnregisterEffector(UEffector* Effector);

	UFUNCTION(BlueprintCallable, Category = "MAPEKit")
	void SetUpdateInterval(float Seconds);

	UFUNCTION(BlueprintCallable, Category = "MAPEKit")
	void StartMAPECycle();

	UFUNCTION(BlueprintCallable, Category = "MAPEKit")
	void StopMAPECycle();

	UFUNCTION(BlueprintCallable, Category = "MAPEKit")
	void SetPerformanceData(FMAPEKPerformanceData Data);


	UFUNCTION(BlueprintCallable, Category = "MAPEKit")
	void LoadSymptomLibrary(const TArray<FSymptomDefinition>& NewSymptoms);

	UFUNCTION(BlueprintCallable, Category = "MAPEKit")
	void SetActionPull(const TArray<FActionPlan>& NewActions);

	UFUNCTION(BlueprintPure, Category = "MAPEKit")
	float GetOverallPerformanceRatio() const;

	UFUNCTION(BlueprintPure, Category = "MAPEKit")
	const FMAPEKPerformanceData& GetLatestPerformanceData() const { return LatestPerformanceData; }

	UFUNCTION(BlueprintCallable, Category = "MAPEKit")
	void ResetSystem();

	UFUNCTION(BlueprintCallable, Category = "MAPEKit")
	void AddSensor(UBaseSensor* Sensor);

	UFUNCTION(BlueprintCallable, Category = "MAPEKit")
	void RemoveSensor(UBaseSensor* Sensor);

protected:
	UPROPERTY()
	TObjectPtr<UMAPEK_Monitor> Monitor;

	UPROPERTY()
	TObjectPtr<UMAPEK_Analyzer> Analyzer;

	UPROPERTY()
	TObjectPtr<UMAPEK_Planner> Planner;

	UPROPERTY()
	TObjectPtr<UMAPEK_Executor> Executor;

	UPROPERTY()
	TObjectPtr<UMAPEK_Knowledge> KnowledgeBase;

	UPROPERTY()
	TObjectPtr<UMAPEK_SymptomLibrary> SymptomLibrary;

	UPROPERTY()
	TObjectPtr<UMAPEK_ActionPull>ActionPull;

	UPROPERTY(BlueprintReadOnly)
	FMAPEKPerformanceData PerformanceData;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UEffector>> RegisteredEffectors;

	float UpdateIntervalSeconds = 2.0f;
	FTimerHandle MAPECycleTimerHandle;

	FMAPEKPerformanceData LatestPerformanceData;

private:
	void RunMAPECycle();
	bool bIsCycleRunning = false;
};
