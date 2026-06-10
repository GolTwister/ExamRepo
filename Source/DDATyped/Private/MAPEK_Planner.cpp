#include "MAPEK_Planner.h"
#include "Logger.h"

void UMAPEK_Planner::Initialize(UMAPEK_Knowledge* InKnowledge, UMAPEK_ActionPull* Action_Pull)
{
    KnowledgeBase = InKnowledge;
    ActionPull = Action_Pull;
    EnsureActionPullExists();
    DDA_LOG(TEXT("Planner: Planner initialized"));
}

void UMAPEK_Planner::Reset()
{
    if (ActionPull)
    {
        ActionPull->Reset();
    }
}

void UMAPEK_Planner::EnsureActionPullExists()
{
    if (!ActionPull)
    {
        ActionPull = NewObject<UMAPEK_ActionPull>(this);
        ActionPull->Initialize();
        DDA_WARNING(TEXT("Planner: No ActionPull existed"));
    }
}

void UMAPEK_Planner::SetActionPull(UMAPEK_ActionPull* NewActionPull)
{
    ActionPull = NewActionPull;
}

FActionPlan UMAPEK_Planner::Plan(const FString& SymptomID)
{
    DDA_LOG(TEXT("Planner: Plan called"));
    EnsureActionPullExists();

    float CurrentRatio = 1.0f;
    if (KnowledgeBase)
    {
        CurrentRatio = KnowledgeBase->GetCurrentPerformanceRatio();
    }

    FActionPlan BestAction = ActionPull->GetBestActionForSymptom(SymptomID, CurrentRatio);

    DDA_LOG(TEXT("Planner: Symptom='%s' -> Action='%s'"), * SymptomID, *BestAction.ActionID);
    return BestAction;
}