#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CMAB_Test.h"
#include "MAPK_CMABTestPull.generated.h"

UCLASS(BlueprintType)
class DDATYPED_API UMAPK_CMABTestPull : public UObject
{
    GENERATED_BODY()

public:
    void Initialize();

    void Reset();


    UFUNCTION(BlueprintCallable, Category = "CMABTestPull")
    void LoadTests(const TArray<FCMABTestDefinition>& InTests);

    UFUNCTION(BlueprintCallable, Category = "CMABTestPull")
    void AddTest(const FCMABTestDefinition& Test);

    UFUNCTION(BlueprintCallable, Category = "CMABTestPull")
    bool RemoveTest(const FString& TestID);

    UFUNCTION(BlueprintCallable, Category = "CMABTestPull")
    void ClearAllTests();


    UFUNCTION(BlueprintCallable, Category = "CMABTestPull")
    FCMABTestDefinition GetTestByID(const FString& TestID) const;

    UFUNCTION(BlueprintCallable, Category = "CMABTestPull")
    const TArray<FCMABTestDefinition>& GetAllTests() const { return Tests; }

    UFUNCTION(BlueprintCallable, Category = "CMABTestPull")
    TArray<FCMABTestDefinition> GetTestsByPlayerType(const FString& PlayerType) const;

    UFUNCTION(BlueprintCallable, Category = "CMABTestPull")
    FCMABTestDefinition GetRandomTest() const;

    UFUNCTION(BlueprintCallable, Category = "CMABTestPull")
    FCMABTestDefinition GetRandomTestByType(const FString& PlayerType) const;

    UFUNCTION(BlueprintPure, Category = "CMABTestPull")
    int32 GetTestCount() const { return Tests.Num(); }

    UFUNCTION(BlueprintPure, Category = "CMABTestPull")
    bool HasTest(const FString& TestID) const;

    UFUNCTION(BlueprintCallable, Category = "CMABTestPull")
    TArray<FString> GetAvailablePlayerTypes() const;

protected:
    void SetupDefaultTests();

private:
    UPROPERTY()
    TArray<FCMABTestDefinition> Tests;

    bool bIsInitialized = false;
};
