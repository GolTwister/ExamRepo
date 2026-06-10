#include "MAPEK_Knowledge.h"


void UMAPEK_Knowledge::Initialize()
{
    ClearHistory();
    CurrentPerformanceRatio = 1.0f;
}

void UMAPEK_Knowledge::Reset()
{
    Initialize();
    ActionEffectiveness.Empty();
}

void UMAPEK_Knowledge::UpdatePerformanceRatio(float Ratio)
{
    CurrentPerformanceRatio = Ratio;
    AddToHistory(Ratio);
}

void UMAPEK_Knowledge::AddToHistory(float Value)
{
    PerformanceHistory.Add(Value);
    if (PerformanceHistory.Num() > MaxHistorySize)
    {
        PerformanceHistory.RemoveAt(0);
    }
}

float UMAPEK_Knowledge::GetAveragePerformanceRatio() const
{
    if (PerformanceHistory.Num() == 0) return CurrentPerformanceRatio;

    float Sum = 0.0f;
    for (float Val : PerformanceHistory)
    {
        Sum += Val;
    }
    return Sum / PerformanceHistory.Num();
}

float UMAPEK_Knowledge::GetPerformanceTrend() const
{
    if (PerformanceHistory.Num() < 3) return 0.0f;

    int32 LastIndex = PerformanceHistory.Num() - 1;
    float OldAvg = (PerformanceHistory[LastIndex - 5] + PerformanceHistory[LastIndex - 4]) / 2.0f;
    float NewAvg = (PerformanceHistory[LastIndex - 1] + PerformanceHistory[LastIndex]) / 2.0f;

    return NewAvg - OldAvg;
}

void UMAPEK_Knowledge::ClearHistory()
{
    PerformanceHistory.Empty();
}