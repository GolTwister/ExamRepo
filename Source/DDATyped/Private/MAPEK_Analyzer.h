// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MAPEK_Subsystem.h"
#include "MAPEK_Analyzer.generated.h"


class UMAPEK_Knowledge;
class UMAPEK_SymptomLibrary;

USTRUCT(BlueprintType)

struct FAnalysisResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString PrimarySymptom;

    UPROPERTY(BlueprintReadOnly)
    float PerformanceRatio = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    float Trend = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    bool bIsInFlowChannel = true;
};

UCLASS()
class UMAPEK_Analyzer : public UObject
{
	GENERATED_BODY()
	
public:
    void Initialize(UMAPEK_Knowledge* InKnowledge, UMAPEK_SymptomLibrary* InSymptomLibrary);
    void Reset();

    UFUNCTION(BlueprintCallable, Category = "Analyzer")
    FString Analyze(FMAPEKPerformanceData PerformanceData);
    UFUNCTION(BlueprintCallable, Category = "Analyzer")
    FString Analyzef(float PerformanceRatio);

    UFUNCTION(BlueprintCallable, Category = "Analyzer")
    FAnalysisResult DeepAnalyze(FMAPEKPerformanceData PerformanceData);

private:
    UPROPERTY()
    TObjectPtr<UMAPEK_Knowledge> KnowledgeBase;

    UPROPERTY()
    TObjectPtr<UMAPEK_SymptomLibrary> SymptomLibrary;

    float FlowLowThreshold = 0.8f;
    float FlowHighThreshold = 1.2f;
    float MinHistoryForTrend = 5;
};
