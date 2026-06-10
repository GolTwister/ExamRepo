
#include "MAPEK_Executor.h"
#include "MAPEK_Knowledge.h"
#include "Effector.h"

DEFINE_LOG_CATEGORY_STATIC(LogExecutor, Log, All);

void UMAPEK_Executor::Initialize(UMAPEK_Knowledge* InKnowledge)
{
    KnowledgeBase = InKnowledge;
    Reset();
    DDA_LOG(TEXT("Executor: Executor initialized"));
}

void UMAPEK_Executor::Reset()
{
    bHasLastAction = false;
    EffectorMap.Empty();
}

void UMAPEK_Executor::BuildEffectorMap(const TArray<UEffector*>& Effectors)
{
    EffectorMap.Empty();
    for (UEffector* Effector : Effectors)
    {
        if (Effector)
        {
            EffectorMap.Add(Effector->EffectorName, Effector);
        }
    }
}

UEffector* UMAPEK_Executor::FindEffectorByName(const FString& Name, const TArray<UEffector*>& Effectors) const
{
    if (EffectorMap.Contains(Name))
    {
        return EffectorMap[Name];
    }

    for (UEffector* Effector : Effectors)
    {
        if (Effector && Effector->EffectorName == Name)
        {
            return Effector;
        }
    }
    return nullptr;
}

void UMAPEK_Executor::Execute(const FActionPlan& Plan, const TArray<UEffector*>& Effectors)
{
    DDA_LOG(TEXT("Executor: Execute called"));
    BuildEffectorMap(Effectors);

    if (Plan.TargetEffectorName == TEXT("None") || Plan.ActionID == TEXT("DoNothing"))
    {
        DDA_LOG(TEXT("Executor: No action needed (DoNothing)"));
        return;
    }

    UEffector* TargetEffector = FindEffectorByName(Plan.TargetEffectorName, Effectors);

    if (!TargetEffector)
    {
        DDA_LOG(TEXT("Executor: Cannot find effector '%s'"), *Plan.TargetEffectorName);
        return;
    }

    DDA_LOG(TEXT("Executor: Applying action '%s' to effector '%s' with value %.2f"),
        *Plan.ActionID, *TargetEffector->EffectorName, Plan.TargetValue);

    if (Plan.Duration > 0.0f)
    {
        TargetEffector->ApplyModifierInterp(Plan.TargetValue, Plan.Duration);
    }
    else
    {
        TargetEffector->ApplyModifier(Plan.TargetValue);
    }

    LastExecutedPlan = Plan;
    bHasLastAction = true;

    if (KnowledgeBase)
    {
        float CurrentEffectiveness = KnowledgeBase->ActionEffectiveness.FindRef(Plan.ActionID);
        KnowledgeBase->ActionEffectiveness.Add(Plan.ActionID, CurrentEffectiveness + 0.1f);
    }
}

void UMAPEK_Executor::ExecuteByActionID(const FString& ActionID, const TArray<UEffector*>& Effectors)
{
    BuildEffectorMap(Effectors);

    DDA_LOG(TEXT("ExecuteByActionID: Full implementation requires ActionPull reference"));
}

void UMAPEK_Executor::UndoLastAction()
{
    if (!bHasLastAction) return;

    float InverseValue = 1.0f / LastExecutedPlan.TargetValue;

    UE_LOG(LogExecutor, Log, TEXT("Executor: Undoing last action '%s'"), *LastExecutedPlan.ActionID);

    bHasLastAction = false;
}
