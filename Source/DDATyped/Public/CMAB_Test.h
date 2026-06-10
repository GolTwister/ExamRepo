#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMAB_Test.generated.h"

USTRUCT(BlueprintType)
struct FCMABTestResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString TestID;

    UPROPERTY(BlueprintReadOnly)
    bool bTriggered = false;

    UPROPERTY(BlueprintReadOnly)
    float SuccessDegree = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float ReactionTime = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> TypeRewards;
};

DECLARE_DELEGATE_OneParam(FOnCMABTestCompleted, const FCMABTestResult&);

USTRUCT(BlueprintType)
struct FCMABTestDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TestID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerType;                     

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DurationSeconds = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ActivationCommand;              

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;                    

    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SuccessThreshold = 0.5f;

    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> DefaultRewards;

    bool IsValid() const { return !TestID.IsEmpty() && !PlayerType.IsEmpty(); }
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DDATYPED_API UCMAB_Test : public UActorComponent
{
	GENERATED_BODY()

public:
    
    void Initialize(const FCMABTestDefinition& InDefinition, UObject* InGameContext);

    void Start();

    void ForceComplete();

    const FCMABTestResult& GetResult() const { return Result; }

    bool IsCompleted() const { return bIsCompleted; }

    bool IsActive() const { return bIsActive && !bIsCompleted; }

    FOnCMABTestCompleted OnCompleted;

    const FCMABTestDefinition& GetDefinition() const { return Definition; }

protected:
    virtual void EvaluateResult();

    void CompleteTest(const FCMABTestResult& InResult);

    template<typename T>
    T* GetGameContext() const { return Cast<T>(GameContext); }

    void SetSuccessDegree(float Degree);

    void SetTriggered(bool bTriggered);

private:
    void OnTimerExpired();
    void CheckGameConditions();

private:
    FCMABTestDefinition Definition;
    FCMABTestResult Result;

    UPROPERTY()
    UObject* GameContext = nullptr;

    bool bIsActive = false;
    bool bIsCompleted = false;
    float TimeRemaining = 0.0f;
    FTimerHandle TimerHandle;

    float TimeSinceLastCheck = 0.0f;
    float CheckInterval = 0.5f;
};
