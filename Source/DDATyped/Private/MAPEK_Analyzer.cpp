// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPEK_Analyzer.h"
#include "MAPEK_Knowledge.h"
#include "MAPEK_SymptomLibrary.h"

void UMAPEK_Analyzer::Initialize(UMAPEK_Knowledge* InKnowledge, UMAPEK_SymptomLibrary* InSymptomLibrary)
{
    KnowledgeBase = InKnowledge;
    SymptomLibrary = InSymptomLibrary;
    Reset();
    DDA_LOG(TEXT("Analyzer: Analyzer initialized"));
}

void UMAPEK_Analyzer::Reset()
{
}


FString UMAPEK_Analyzer::Analyze(FMAPEKPerformanceData PerformanceData) {

    DDA_LOG(TEXT("Analyzer: Analyzing performance data"));
    if (!SymptomLibrary)
    {
        if (PerformanceData.Ratio < 0.6f) return TEXT("TooHard");
        if (PerformanceData.Ratio < 0.9f) return TEXT("SlightlyHard");
        if (PerformanceData.Ratio > 1.4f) return TEXT("TooEasy");
        if (PerformanceData.Ratio > 1.1f) return TEXT("SlightlyEasy");
        return TEXT("Flow");
    }

    return SymptomLibrary->GetSymptomForPerformance(PerformanceData);
}

FString UMAPEK_Analyzer::Analyzef(float PerformanceRatio)
{
    DDA_LOG(TEXT("Analyzer: Analyzing performance coeff"));
    if (!SymptomLibrary)
    {
        if (PerformanceRatio < 0.6f) return TEXT("TooHard");
        if (PerformanceRatio < 0.9f) return TEXT("SlightlyHard");
        if (PerformanceRatio > 1.4f) return TEXT("TooEasy");
        if (PerformanceRatio > 1.1f) return TEXT("SlightlyEasy");
        return TEXT("Flow");
    }

    return TEXT("None");
    //return SymptomLibrary->GetSymptomForPerformance(PerformanceRatio);
}

FAnalysisResult UMAPEK_Analyzer::DeepAnalyze(FMAPEKPerformanceData PerformanceData)
{
    DDA_LOG(TEXT("Analyzer: Analyze with deep algorithm"));
    FAnalysisResult Result;
    Result.PerformanceRatio = PerformanceData.Ratio;

    Result.PrimarySymptom = Analyzef(PerformanceData.Ratio);

    if (KnowledgeBase)
    {
        Result.Trend = KnowledgeBase->GetPerformanceTrend();
    }

    Result.bIsInFlowChannel = (PerformanceData.Ratio >= FlowLowThreshold &&
        PerformanceData.Ratio <= FlowHighThreshold);

    return Result;
}
