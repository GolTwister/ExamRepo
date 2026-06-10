#include "MAPEK_SymptomLibrary.h"
#include "MAPEK_Subsystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogSymptomLibrary, Log, All);

void UMAPEK_SymptomLibrary::Initialize()
{
    SetupDefaultSymptoms();
    UE_LOG(LogSymptomLibrary, Log, TEXT("SymptomLibrary initialized with %d symptoms"), Symptoms.Num());
}

void UMAPEK_SymptomLibrary::Reset()
{
    Symptoms.Empty();
    SetupDefaultSymptoms();
}

void UMAPEK_SymptomLibrary::SetupDefaultSymptoms()
{
}

void UMAPEK_SymptomLibrary::LoadSymptoms(const TArray<FSymptomDefinition>& NewSymptoms)
{
    Symptoms = NewSymptoms;
    DDA_VERBOSE(TEXT("Symptoms loaded. Symptoms num: %d"), Symptoms.Num());
}

void UMAPEK_SymptomLibrary::AddSymptom(const FSymptomDefinition& Symptom)
{
    Symptoms.Add(Symptom);
}

void UMAPEK_SymptomLibrary::LoadRules(const TArray<FDiagnoseRule>& NewRules)
{
    Rules = NewRules;
}

void UMAPEK_SymptomLibrary::AddRules(const FDiagnoseRule& Rule)
{
    Rules.Add(Rule);
}


FString UMAPEK_SymptomLibrary::GetSymptomForPerformance(FMAPEKPerformanceData PerformanceData) const
{
    FString BestMatch;
    int32 HighestPriority = -1;

    for (const FSymptomDefinition& Symptom : Symptoms)
    {
        if (PerformanceData.Ratio >= Symptom.MinPerformanceRatio && PerformanceData.Ratio <= Symptom.MaxPerformanceRatio)
        {
            if (Symptom.Priority > HighestPriority)
            {
                HighestPriority = Symptom.Priority;
                BestMatch = Symptom.SymptomID;
            }
        }
    }

    if (BestMatch.IsEmpty())
    {
        return TEXT("Unknown");
    }

    return BestMatch;
}