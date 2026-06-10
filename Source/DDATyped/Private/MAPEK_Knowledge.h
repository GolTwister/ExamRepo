#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MAPEK_Knowledge.generated.h"

struct Param {
    FString Name;
    float value;
};

struct ParamsGroup {
    FString GroupName;
    TArray<Param> Params;
};

UCLASS(BlueprintType)
class UMAPEK_Knowledge : public UObject
{
	GENERATED_BODY()

public:
    void Initialize();
    void Reset();

    UFUNCTION(BlueprintCallable, Category = "Knowledge")
    void UpdatePerformanceRatio(float Ratio);

    UFUNCTION(BlueprintPure, Category = "Knowledge")
    float GetCurrentPerformanceRatio() const { return CurrentPerformanceRatio; }

    UFUNCTION(BlueprintPure, Category = "Knowledge")
    float GetAveragePerformanceRatio() const;

    UFUNCTION(BlueprintPure, Category = "Knowledge")
    float GetPerformanceTrend() const; 

    UFUNCTION(BlueprintCallable, Category = "Knowledge")
    void AddToHistory(float Value);

    UFUNCTION(BlueprintPure, Category = "Knowledge")
    const TArray<float>& GetPerformanceHistory() const { return PerformanceHistory; }

    UFUNCTION(BlueprintCallable, Category = "Knowledge")
    void ClearHistory();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Knowledge")
    TMap<FString, float> ActionEffectiveness;


private:
    UPROPERTY()
    float CurrentPerformanceRatio = 1.0f;

    UPROPERTY()
    TArray<float> PerformanceHistory;

    UPROPERTY()
    int32 MaxHistorySize = 30;

    UPROPERTY()
    TArray<FString> ParGroups;

};
