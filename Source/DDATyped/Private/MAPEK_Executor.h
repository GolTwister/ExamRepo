// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MAPEK_ActionPull.h"
#include "Logger.h"

#include "MAPEK_Executor.generated.h"

class UMAPEK_Knowledge;
class UEffector;

UCLASS()
class UMAPEK_Executor : public UObject
{
	GENERATED_BODY()

public:
    void Initialize(UMAPEK_Knowledge* InKnowledge);
    void Reset();

    UFUNCTION(BlueprintCallable, Category = "Executor")
    void Execute(const FActionPlan& Plan, const TArray<UEffector*>& Effectors);

    UFUNCTION(BlueprintCallable, Category = "Executor")
    void ExecuteByActionID(const FString& ActionID, const TArray<UEffector*>& Effectors);

    UFUNCTION(BlueprintCallable, Category = "Executor")
    void UndoLastAction();

private:
    UPROPERTY()
    TObjectPtr<UMAPEK_Knowledge> KnowledgeBase;

    TMap<FString, UEffector*> EffectorMap;
    FActionPlan LastExecutedPlan;
    bool bHasLastAction = false;

    UEffector* FindEffectorByName(const FString& Name, const TArray<UEffector*>& Effectors) const;
    void BuildEffectorMap(const TArray<UEffector*>& Effectors);
};
