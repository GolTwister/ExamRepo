#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/GameplayStatics.h"
#include "MAPEK_Monitor.generated.h"

class UMAPEK_Knowledge;
class UMAPEK_Subsystem;

UCLASS()
class UMAPEK_Monitor : public UObject
{
	GENERATED_BODY()
public:
    void Initialize(UMAPEK_Subsystem* InSubsystem, UMAPEK_Knowledge* InKnowledge);
    void Reset();
    void Update(float DeltaTime);

    void Collect();

    UFUNCTION(BlueprintCallable, Category = "MAPEKit")
    void RegisterSensor(UBaseSensor* Sensor);

    UFUNCTION(BlueprintCallable, Category = "MAPEKit")
    void UnregisterSensor(UBaseSensor* Sensor);

protected:

    UPROPERTY(BlueprintReadOnly)
    TArray<TObjectPtr<UBaseSensor>> RegisteredSensors;

private:

    UPROPERTY()
    TObjectPtr<UMAPEK_Subsystem> Subsystem;

    UPROPERTY()
    TObjectPtr<UMAPEK_Knowledge> KnowledgeBase;


    float TimeSinceLastCollect = 0.0f;
    float CollectInterval = 1.0f;
};