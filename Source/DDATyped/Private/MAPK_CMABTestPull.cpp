#include "MAPK_CMABTestPull.h"

DEFINE_LOG_CATEGORY_STATIC(LogCMABTestPull, Log, All);

void UMAPK_CMABTestPull::Initialize()
{
    if (bIsInitialized) return;

    SetupDefaultTests();
    bIsInitialized = true;

    UE_LOG(LogCMABTestPull, Log, TEXT("CMABTestPull initialized with %d tests"), Tests.Num());
}

void UMAPK_CMABTestPull::Reset()
{
    Tests.Empty();
    SetupDefaultTests();
    UE_LOG(LogCMABTestPull, Log, TEXT("CMABTestPull reset"));
}

void UMAPK_CMABTestPull::LoadTests(const TArray<FCMABTestDefinition>& InTests)
{
    Tests = InTests;
    UE_LOG(LogCMABTestPull, Log, TEXT("Loaded %d tests into CMABTestPull"), Tests.Num());
}

void UMAPK_CMABTestPull::AddTest(const FCMABTestDefinition& Test)
{
    if (!Test.IsValid())
    {
        UE_LOG(LogCMABTestPull, Warning, TEXT("Cannot add invalid test"));
        return;
    }

    for (const FCMABTestDefinition& Existing : Tests)
    {
        if (Existing.TestID == Test.TestID)
        {
            UE_LOG(LogCMABTestPull, Warning, TEXT("Test with ID '%s' already exists, skipping"), *Test.TestID);
            return;
        }
    }

    Tests.Add(Test);
    UE_LOG(LogCMABTestPull, Verbose, TEXT("Added test '%s' (Type=%s)"), *Test.TestID, *Test.PlayerType);
}

bool UMAPK_CMABTestPull::RemoveTest(const FString& TestID)
{
    for (int32 i = 0; i < Tests.Num(); i++)
    {
        if (Tests[i].TestID == TestID)
        {
            Tests.RemoveAt(i);
            UE_LOG(LogCMABTestPull, Verbose, TEXT("Removed test '%s'"), *TestID);
            return true;
        }
    }

    UE_LOG(LogCMABTestPull, Warning, TEXT("Test '%s' not found for removal"), *TestID);
    return false;
}

void UMAPK_CMABTestPull::ClearAllTests()
{
    Tests.Empty();
    UE_LOG(LogCMABTestPull, Log, TEXT("Cleared all tests"));
}

FCMABTestDefinition UMAPK_CMABTestPull::GetTestByID(const FString& TestID) const
{
    for (const FCMABTestDefinition& Test : Tests)
    {
        if (Test.TestID == TestID)
        {
            return Test;
        }
    }

    UE_LOG(LogCMABTestPull, Warning, TEXT("Test '%s' not found"), *TestID);
    return FCMABTestDefinition();
}

TArray<FCMABTestDefinition> UMAPK_CMABTestPull::GetTestsByPlayerType(const FString& PlayerType) const
{
    TArray<FCMABTestDefinition> Result;

    for (const FCMABTestDefinition& Test : Tests)
    {
        if (Test.PlayerType == PlayerType)
        {
            Result.Add(Test);
        }
    }

    return Result;
}

FCMABTestDefinition UMAPK_CMABTestPull::GetRandomTest() const
{
    if (Tests.Num() == 0)
    {
        UE_LOG(LogCMABTestPull, Warning, TEXT("GetRandomTest called but no tests available"));
        return FCMABTestDefinition();
    }

    int32 RandomIndex = FMath::RandRange(0, Tests.Num() - 1);
    return Tests[RandomIndex];
}

FCMABTestDefinition UMAPK_CMABTestPull::GetRandomTestByType(const FString& PlayerType) const
{
    TArray<FCMABTestDefinition> TypedTests = GetTestsByPlayerType(PlayerType);

    if (TypedTests.Num() == 0)
    {
        UE_LOG(LogCMABTestPull, Warning, TEXT("GetRandomTestByType: no tests for type '%s'"), *PlayerType);
        return FCMABTestDefinition();
    }

    int32 RandomIndex = FMath::RandRange(0, TypedTests.Num() - 1);
    return TypedTests[RandomIndex];
}

bool UMAPK_CMABTestPull::HasTest(const FString& TestID) const
{
    for (const FCMABTestDefinition& Test : Tests)
    {
        if (Test.TestID == TestID)
        {
            return true;
        }
    }
    return false;
}

TArray<FString> UMAPK_CMABTestPull::GetAvailablePlayerTypes() const
{
    TSet<FString> UniqueTypes;

    for (const FCMABTestDefinition& Test : Tests)
    {
        if (!Test.PlayerType.IsEmpty())
        {
            UniqueTypes.Add(Test.PlayerType);
        }
    }

    return UniqueTypes.Array();
}

void UMAPK_CMABTestPull::SetupDefaultTests()
{
    
}