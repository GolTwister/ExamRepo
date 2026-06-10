#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
//#include "MAPEK_Subsystem.h"
#include "MAPEK_SymptomLibrary.generated.h"


USTRUCT(BlueprintType)
struct FDiagnoseRule
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    /*Use "General" in string to analize PerformanceData.Ratio*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int> Symptoms;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Priority = 0;
};

UCLASS(BlueprintType)
class DDATYPED_API UMAPEK_SymptomLibrary : public UObject
{
	GENERATED_BODY()

public:
    void Initialize();
    void Reset();

    UFUNCTION(BlueprintCallable, Category = "SymptomLibrary")
    void LoadSymptoms(const TArray<FSymptomDefinition>& Symptoms);

    UFUNCTION(BlueprintCallable, Category = "SymptomLibrary")
    void AddSymptom(const FSymptomDefinition& Symptom);

    UFUNCTION(BlueprintCallable, Category = "SymptomLibrary")
    void LoadRules(const TArray<FDiagnoseRule>& NewRules);

    UFUNCTION(BlueprintCallable, Category = "SymptomLibrary")
    void AddRules(const FDiagnoseRule& Rule);

    UFUNCTION(BlueprintCallable, Category = "SymptomLibrary")
    FString GetSymptomForPerformance(FMAPEKPerformanceData PerformanceData) const;

    UFUNCTION(BlueprintPure, Category = "SymptomLibrary")
    const TArray<FSymptomDefinition>& GetAllSymptoms() const { return Symptoms; }


private:
    UPROPERTY()
    TArray<FSymptomDefinition> Symptoms;

    UPROPERTY()
    TArray<FDiagnoseRule> Rules;

    void SetupDefaultSymptoms();
};
