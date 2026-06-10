#include "MAPEK_CMAB.h"
#include "MAPEK_Knowledge.h"

DEFINE_LOG_CATEGORY_STATIC(LogCMAB, Log, All);


void FCMABPlayerProfile::UpdateScore(const FString& Type, float Reward, float LearningRate)
{
    float CurrentScore = TypeScores.FindRef(Type);
    float NewScore = CurrentScore + LearningRate * (Reward - CurrentScore);
    NewScore = FMath::Clamp(NewScore, 0.0f, 1.0f);
    TypeScores.Add(Type, NewScore);

    int32 Attempts = TypeAttempts.FindRef(Type);
    TypeAttempts.Add(Type, Attempts + 1);
}

FString FCMABPlayerProfile::GetDominantType() const
{
    FString BestType;
    float BestScore = -1.0f;
    for (const auto& Pair : TypeScores)
    {
        if (Pair.Value > BestScore)
        {
            BestScore = Pair.Value;
            BestType = Pair.Key;
        }
    }
    return BestType;
}

float FCMABPlayerProfile::GetScore(const FString& Type) const
{
    return TypeScores.FindRef(Type);
}


void UMAPEK_CMAB::Initialize(UMAPEK_Knowledge* InKnowledgeBase)
{
    KnowledgeBase = InKnowledgeBase;
    TimeSinceLastTest = 0.0f;
    ConsecutiveFlowCycles = 0;
    ActiveTest.bIsActive = false;
    bIsInitialized = true;

    UE_LOG(LogCMAB, Log, TEXT("CMAB initialized (Epsilon=%.2f, Interval=%.1fs)"), Epsilon, TestIntervalSeconds);
}

void UMAPEK_CMAB::Shutdown()
{
    CancelActiveTest();
    bIsInitialized = false;
    UE_LOG(LogCMAB, Log, TEXT("CMAB shutdown"));
}

void UMAPEK_CMAB::Tick(float DeltaTime)
{
    if (!bIsInitialized) return;

    if (ActiveTest.bIsActive)
    {
        float CurrentTime = FPlatformTime::Seconds();
        float Elapsed = CurrentTime - ActiveTest.StartTime;
        if (Elapsed >= ActiveTest.TestDefinition.DurationSeconds)
        {
            UE_LOG(LogCMAB, Verbose, TEXT("Test '%s' timed out"), *ActiveTest.TestDefinition.TestID);
            CompleteActiveTest(false, 0.0f);
        }
    }
    else
    {
        TimeSinceLastTest += DeltaTime;
        if (TimeSinceLastTest >= TestIntervalSeconds)
        {
            TryStartTest();
        }
    }
}

void UMAPEK_CMAB::TryStartTest()
{
    if (!bIsInitialized)
    {
        UE_LOG(LogCMAB, Warning, TEXT("TryStartTest called but CMAB not initialized"));
        return;
    }

    if (ActiveTest.bIsActive)
    {
        UE_LOG(LogCMAB, Verbose, TEXT("Cannot start test: another test is already active"));
        return;
    }

    if (!TestPull)
    {
        UE_LOG(LogCMAB, Warning, TEXT("Cannot start test: TestPull not set"));
        return;
    }

    if (ConsecutiveFlowCycles < MinFlowCyclesBeforeTest)
    {
        TimeSinceLastTest = TestIntervalSeconds * 0.5f;
        return;
    }

    FCMABTestDefinition SelectedTest = SelectTestByEpsilonGreedy();

    if (SelectedTest.TestID.IsEmpty())
    {
        UE_LOG(LogCMAB, Warning, TEXT("No tests available in TestPull"));
        return;
    }

    ActiveTest.TestDefinition = SelectedTest;
    ActiveTest.StartTime = FPlatformTime::Seconds();
    ActiveTest.bIsActive = true;
    TimeSinceLastTest = 0.0f;

    ActivateTestInGame(SelectedTest);

    UE_LOG(LogCMAB, Log, TEXT("Started test '%s' (Type=%s, Duration=%.1fs)"),
        *SelectedTest.TestID, *SelectedTest.PlayerType, SelectedTest.DurationSeconds);
}

FCMABTestDefinition UMAPEK_CMAB::SelectTestByEpsilonGreedy()
{
    if (!TestPull)
    {
        return FCMABTestDefinition();
    }

    TArray<FCMABTestDefinition> AllTests = TestPull->GetAllTests();
    if (AllTests.Num() == 0)
    {
        return FCMABTestDefinition();
    }

    float RandomValue = FMath::FRand();

    if (RandomValue < Epsilon)
    {
        int32 RandomIndex = FMath::RandRange(0, AllTests.Num() - 1);
        UE_LOG(LogCMAB, Verbose, TEXT("Exploration: selecting random test"));
        return AllTests[RandomIndex];
    }
    else
    {
        FCMABTestDefinition BestTest;
        float BestScore = -1.0f;

        for (const FCMABTestDefinition& Test : AllTests)
        {
            float TestScore = PlayerProfile.GetScore(Test.PlayerType);
            if (TestScore > BestScore)
            {
                BestScore = TestScore;
                BestTest = Test;
            }
        }

        if (BestTest.TestID.IsEmpty())
        {
            return AllTests[0];
        }

        UE_LOG(LogCMAB, Verbose, TEXT("Exploitation: selecting test '%s' with score %.2f"),
            *BestTest.TestID, BestScore);
        return BestTest;
    }
}

void UMAPEK_CMAB::ActivateTestInGame(const FCMABTestDefinition& Test)
{
    if (!EffectorTarget)
    {
        UE_LOG(LogCMAB, Warning, TEXT("Cannot activate test: EffectorTarget not set"));
        return;
    }

    UE_LOG(LogCMAB, Verbose, TEXT("Activating test in game: %s"), *Test.ActivationCommand);
}

void UMAPEK_CMAB::CompleteActiveTest(bool bTriggered, float SuccessDegree, float ReactionTime)
{
    if (!ActiveTest.bIsActive)
    {
        UE_LOG(LogCMAB, Warning, TEXT("CompleteActiveTest called but no active test"));
        return;
    }

    float Reward = ComputeReward(bTriggered, SuccessDegree);
    UpdatePlayerProfile(ActiveTest.TestDefinition, Reward);

    UE_LOG(LogCMAB, Log, TEXT("Test '%s' completed: Triggered=%s, Success=%.2f, Reward=%.2f"),
        *ActiveTest.TestDefinition.TestID,
        bTriggered ? TEXT("yes") : TEXT("no"),
        SuccessDegree, Reward);

    ActiveTest.bIsActive = false;
}

void UMAPEK_CMAB::CancelActiveTest()
{
    if (ActiveTest.bIsActive)
    {
        UE_LOG(LogCMAB, Log, TEXT("Test '%s' cancelled"), *ActiveTest.TestDefinition.TestID);
        ActiveTest.bIsActive = false;
    }
}

float UMAPEK_CMAB::ComputeReward(bool bTriggered, float SuccessDegree) const
{
    if (!bTriggered) return 0.0f;

    return FMath::Clamp(SuccessDegree, 0.0f, 1.0f);
}

void UMAPEK_CMAB::UpdatePlayerProfile(const FCMABTestDefinition& Test, float Reward)
{
    PlayerProfile.UpdateScore(Test.PlayerType, Reward, LearningRate);

    UE_LOG(LogCMAB, Verbose, TEXT("Updated profile: %s type score = %.2f"),
        *Test.PlayerType, PlayerProfile.GetScore(Test.PlayerType));

    if (KnowledgeBase)
    {
    }
}
