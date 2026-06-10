#include "CMAB_Test.h"
#include "TimerManager.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogCMABTest, Log, All);

void UCMAB_Test::Initialize(const FCMABTestDefinition& InDefinition, UObject* InGameContext)
{
    Definition = InDefinition;
    GameContext = InGameContext;

    Result.TestID = Definition.TestID;
    Result.bTriggered = false;
    Result.SuccessDegree = 0.0f;
    Result.ReactionTime = 0.0f;

    for (const auto& Pair : Definition.DefaultRewards)
    {
        Result.TypeRewards.Add(Pair.Key, Pair.Value);
    }

    bIsActive = false;
    bIsCompleted = false;

    UE_LOG(LogCMABTest, Verbose, TEXT("Test '%s' initialized"), *Definition.TestID);
}

void UCMAB_Test::Start()
{
    if (bIsActive || bIsCompleted)
    {
        UE_LOG(LogCMABTest, Warning, TEXT("Test '%s' cannot be started (already active/completed)"), *Definition.TestID);
        return;
    }

    bIsActive = true;
    TimeRemaining = Definition.DurationSeconds;

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(TimerHandle, this, &UCMAB_Test::OnTimerExpired, Definition.DurationSeconds, false);
        UE_LOG(LogCMABTest, Log, TEXT("Test '%s' started (duration=%.1fs)"), *Definition.TestID, Definition.DurationSeconds);
    }
    else
    {
        UE_LOG(LogCMABTest, Error, TEXT("Test '%s' cannot start: no world"), *Definition.TestID);
        CompleteTest(Result);
    }

    CheckGameConditions();
}

void UCMAB_Test::ForceComplete()
{
    if (!bIsActive || bIsCompleted) return;

    UE_LOG(LogCMABTest, Log, TEXT("Test '%s' force completed"), *Definition.TestID);
    EvaluateResult();
}

void UCMAB_Test::CheckGameConditions()
{
    if (Result.bTriggered)
    {
        CompleteTest(Result);
    }
}

void UCMAB_Test::OnTimerExpired()
{
    if (!bIsActive || bIsCompleted) return;

    UE_LOG(LogCMABTest, Verbose, TEXT("Test '%s' timer expired"), *Definition.TestID);
    EvaluateResult();
}

void UCMAB_Test::EvaluateResult()
{

    CompleteTest(Result);
}

void UCMAB_Test::CompleteTest(const FCMABTestResult& InResult)
{
    if (bIsCompleted) return;

    Result = InResult;
    bIsActive = false;
    bIsCompleted = true;

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(TimerHandle);
    }

    UE_LOG(LogCMABTest, Log, TEXT("Test '%s' completed: Triggered=%s, SuccessDegree=%.2f"),
        *Definition.TestID,
        Result.bTriggered ? TEXT("yes") : TEXT("no"),
        Result.SuccessDegree);

    OnCompleted.ExecuteIfBound(Result);
}

void UCMAB_Test::SetTriggered(bool bTriggered)
{
    if (!bIsActive || bIsCompleted) return;
    Result.bTriggered = bTriggered;
}

void UCMAB_Test::SetSuccessDegree(float Degree)
{
    if (!bIsActive || bIsCompleted) return;
    Result.SuccessDegree = FMath::Clamp(Degree, 0.0f, 1.0f);
}