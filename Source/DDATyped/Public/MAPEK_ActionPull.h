#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MAPEK_ActionPull.generated.h"

USTRUCT(BlueprintType)
struct FActionPlan : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ActionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetEffectorName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TargetValue = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Priority = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ApplicableSymptoms;

    bool IsApplicableForSymptom(const FString& SymptomID) const
    {
        return ApplicableSymptoms.Contains(SymptomID) || ApplicableSymptoms.Contains(TEXT("All"));
    }
};

UCLASS()
class DDATYPED_API UMAPEK_ActionPull : public UObject
{
	GENERATED_BODY()
	
public:
    void Initialize();
    void Reset();

    UFUNCTION(BlueprintCallable, Category = "ActionPull")
    void LoadActions(const TArray<FActionPlan>& NewActions);

    UFUNCTION(BlueprintCallable, Category = "ActionPull")
    void AddAction(const FActionPlan& Action);

    UFUNCTION(BlueprintCallable, Category = "ActionPull")
    TArray<FActionPlan> GetActionsForSymptom(const FString& SymptomID) const;

    UFUNCTION(BlueprintCallable, Category = "ActionPull")
    FActionPlan GetBestActionForSymptom(const FString& SymptomID, float CurrentPerformanceRatio) const;

private:
    UPROPERTY()
    TArray<FActionPlan> Actions;
};
