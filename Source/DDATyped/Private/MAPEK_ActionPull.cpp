// Fill out your copyright notice in the Description page of Project Settings.


#include "MAPEK_ActionPull.h"
#include "Logger.h"

DEFINE_LOG_CATEGORY_STATIC(LogActionPull, Log, All);

void UMAPEK_ActionPull::Initialize()
{
    DDA_LOG(TEXT("ActionPull: ActionPull initialized"));
}

void UMAPEK_ActionPull::Reset()
{
    Actions.Empty();
    DDA_LOG(TEXT("ActionPull: ActionPull reset"));
}

void UMAPEK_ActionPull::LoadActions(const TArray<FActionPlan>& NewActions)
{
    Actions = NewActions;
    DDA_LOG(TEXT("ActionPull: ActionPull loaded. Actions num: %d"), Actions.Num());
}

void UMAPEK_ActionPull::AddAction(const FActionPlan& Action)
{
    Actions.Add(Action);
}

TArray<FActionPlan> UMAPEK_ActionPull::GetActionsForSymptom(const FString& SymptomID) const
{
    TArray<FActionPlan> Result;
    for (const FActionPlan& Action : Actions)
    {
        if (Action.IsApplicableForSymptom(SymptomID))
        {
            Result.Add(Action);
        }
        DDA_VERBOSE(TEXT("Action %s is applicable for symptom %s : %d"), *Action.ActionID, *SymptomID, Action.IsApplicableForSymptom(SymptomID));
    }
    DDA_VERBOSE(TEXT("ActionPull: Actions num: %d"), Actions.Num());
    return Result;
}

FActionPlan UMAPEK_ActionPull::GetBestActionForSymptom(const FString& SymptomID, float CurrentPerformanceRatio) const
{
    TArray<FActionPlan> ApplicableActions = GetActionsForSymptom(SymptomID);

    if (ApplicableActions.Num() == 0)
    {
        FActionPlan DefaultAction;
        DefaultAction.ActionID = TEXT("DoNothing");
        DefaultAction.TargetEffectorName = TEXT("None");
        return DefaultAction;
    }

    ApplicableActions.Sort([](const FActionPlan& A, const FActionPlan& B) {
        return A.Priority > B.Priority;
        });

    return ApplicableActions[0];
}