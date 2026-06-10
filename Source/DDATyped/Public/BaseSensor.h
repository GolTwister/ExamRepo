#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MAPEK_Subsystem.h"
#include "BaseSensor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMeasureMetric, float, DeltaTime);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DDATYPED_API UBaseSensor : public UActorComponent
{
	GENERATED_BODY()

public:
    UBaseSensor();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensor")
    FString SensorName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensor")
    bool CollectWithUpdate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sensor")
    float MeasureWeight = 1.0f;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sensor")
    float CollectMetric();
    virtual float CollectMetric_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sensor")
    float GetTargetMetric();
    virtual float GetTargetMetric_Implementation();

    UFUNCTION(BlueprintCallable, Category = "Sensor")
    void MeasureMetric(float DeltaTime);

    //Applying Modifier by Effector
    UPROPERTY(BlueprintAssignable, Category = "Effector")
    FOnMeasureMetric OnMeasureMetric;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Sensor")
    void ResetSensor();
    virtual void ResetSensor_Implementation();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sensor")
    float CurrentMetric = 0.0f;
		
};
