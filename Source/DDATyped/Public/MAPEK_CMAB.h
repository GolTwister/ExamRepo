#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CMAB_Test.h"
#include "MAPK_CMABTestPull.h"
#include "MAPEK_CMAB.generated.h"

class UMAPEK_Knowledge;

USTRUCT()
struct FActiveCMABTest
{
    GENERATED_BODY()

    UPROPERTY()
    FCMABTestDefinition TestDefinition;

    UPROPERTY()
    float StartTime = 0.0f;

    UPROPERTY()
    bool bIsActive = false;
};

USTRUCT()
struct FCMABPlayerProfile
{
    GENERATED_BODY()

    UPROPERTY()
    TMap<FString, float> TypeScores;

    UPROPERTY()
    TMap<FString, int32> TypeAttempts;

    void UpdateScore(const FString& Type, float Reward, float LearningRate = 0.3f);
    FString GetDominantType() const;
    float GetScore(const FString& Type) const;
};


UCLASS()
class DDATYPED_API UMAPEK_CMAB : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(UMAPEK_Knowledge* InKnowledgeBase);
    void Shutdown();
    void Tick(float DeltaTime);

    void TryStartTest();

    void CompleteActiveTest(bool bTriggered, float SuccessDegree, float ReactionTime = 0.0f);

    void CancelActiveTest();

    UPROPERTY(EditAnywhere, Category = "CMAB")
    float TestIntervalSeconds = 60.0f;

    UPROPERTY(EditAnywhere, Category = "CMAB")
    float Epsilon = 0.2f;                   

    UPROPERTY(EditAnywhere, Category = "CMAB")
    float LearningRate = 0.3f;             

    UPROPERTY(EditAnywhere, Category = "CMAB")
    int32 MinFlowCyclesBeforeTest = 3;       

    const FCMABPlayerProfile& GetPlayerProfile() const { return PlayerProfile; }

    UFUNCTION(BlueprintPure, Category = "CMAB")
    bool HasActiveTest() const { return ActiveTest.bIsActive; }

    UFUNCTION(BlueprintPure, Category = "CMAB")
    FString GetCurrentTestID() const { return ActiveTest.TestDefinition.TestID; }

    void SetTestPull(UMAPK_CMABTestPull* InTestPull) { TestPull = InTestPull; }
    void SetEffectorTarget(UObject* InEffectorTarget) { EffectorTarget = InEffectorTarget; }

protected:
    
    FCMABTestDefinition SelectTestByEpsilonGreedy();

    
    void ActivateTestInGame(const FCMABTestDefinition& Test);

    
    void UpdatePlayerProfile(const FCMABTestDefinition& Test, float Reward);

    
    float ComputeReward(bool bTriggered, float SuccessDegree) const;

private:
    UPROPERTY()
    UMAPEK_Knowledge* KnowledgeBase = nullptr;

    UPROPERTY()
    UMAPK_CMABTestPull* TestPull = nullptr;

    UPROPERTY()
    UObject* EffectorTarget = nullptr;  

    FCMABPlayerProfile PlayerProfile;
    FActiveCMABTest ActiveTest;
    float TimeSinceLastTest = 0.0f;
    int32 ConsecutiveFlowCycles = 0;
    bool bIsInitialized = false;
};