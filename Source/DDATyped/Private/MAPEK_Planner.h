#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MAPEK_ActionPull.h"
#include "MAPEK_Knowledge.h"
#include "MAPEK_Planner.generated.h"

class UMAPEK_Knowledge;

UCLASS()
class UMAPEK_Planner : public UObject
{
	GENERATED_BODY()

public:
    void Initialize(UMAPEK_Knowledge* InKnowledge, UMAPEK_ActionPull* ActionPull);
    void Reset();

    UFUNCTION(BlueprintCallable, Category = "Planner")
    FActionPlan Plan(const FString& SymptomID);

    UFUNCTION(BlueprintCallable, Category = "Planner")
    void SetActionPull(UMAPEK_ActionPull* NewActionPull);

private:
    UPROPERTY()
    TObjectPtr<UMAPEK_Knowledge> KnowledgeBase;

    UPROPERTY()
    TObjectPtr<UMAPEK_ActionPull> ActionPull;

    void EnsureActionPullExists();
};
