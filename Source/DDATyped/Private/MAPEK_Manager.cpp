#include "MAPEK_Manager.h"
#include "MAPEK_Subsystem.h"

AMAPEK_Manager::AMAPEK_Manager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMAPEK_Manager::BeginPlay()
{
    Super::BeginPlay();

    bIsDataTableValid = ValidateSymptomTable();
    UGameInstance* GameInstance = GetWorld()->GetGameInstance();

    if (bIsDataTableValid)
    {
        UE_LOG(LogTemp, Log, TEXT("Symptom DataTable is valid, loaded %d entries"),
            SymptomDataTable->GetRowNames().Num());
        if (GameInstance)
        {
            UMAPEK_Subsystem* Subsystem = GameInstance->GetSubsystem<UMAPEK_Subsystem>();
            if (Subsystem)
            {
                Subsystem->LoadSymptomLibrary(GetAllSymptoms());
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Symptom DataTable is invalid or missing"));
    }

    bIsDataTableValid = ValidateActionTable();

    if (bIsDataTableValid)
    {
        UE_LOG(LogTemp, Log, TEXT("Symptom DataTable is valid, loaded %d entries"),
            ActionDataTable->GetRowNames().Num());
        if (GameInstance)
        {
            UMAPEK_Subsystem* Subsystem = GameInstance->GetSubsystem<UMAPEK_Subsystem>();
            if (Subsystem)
            {
                Subsystem->SetActionPull(GetAllActions());
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Symptom DataTable is invalid or missing"));
    }
}

void AMAPEK_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AMAPEK_Manager::ValidateSymptomTable() const
{
    if (!SymptomDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("SymptomDataTable is not assigned"));
        return false;
    }

    // Проверяем, что структура таблицы соответствует FSymptomDefinition
    const UScriptStruct* RowStruct = SymptomDataTable->GetRowStruct();
    if (!RowStruct || !RowStruct->IsChildOf(FSymptomDefinition::StaticStruct()))
    {
        UE_LOG(LogTemp, Error, TEXT("DataTable structure mismatch! Expected: FSymptomDefinition, Got: %s"),
            RowStruct ? *RowStruct->GetName() : TEXT("null"));
        return false;
    }

    return true;
}

TArray<FSymptomDefinition> AMAPEK_Manager::GetAllSymptoms() const
{
    TArray<FSymptomDefinition> Results;

    if (!ValidateSymptomTable())
    {
        return Results;
    }

    // Получаем все имена строк
    TArray<FName> RowNames = SymptomDataTable->GetRowNames();

    for (const FName& RowName : RowNames)
    {
        // Ищем строку по имени
        FSymptomDefinition* Row = SymptomDataTable->FindRow<FSymptomDefinition>(RowName, TEXT("GetAllSymptoms"));
        if (Row)
        {
            Results.Add(*Row);
        }
    }

    return Results;
}

bool AMAPEK_Manager::ValidateActionTable() const {
    if (!ActionDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("ActionDataTable is not assigned"));
        return false;
    }

    const UScriptStruct* RowStruct = ActionDataTable->GetRowStruct();
    if (!RowStruct || !RowStruct->IsChildOf(FActionPlan::StaticStruct()))
    {
        UE_LOG(LogTemp, Error, TEXT("DataTable structure mismatch! Expected: FActionPlan, Got: %s"),
            RowStruct ? *RowStruct->GetName() : TEXT("null"));
        return false;
    }

    return true;
};

TArray<FActionPlan> AMAPEK_Manager::GetAllActions() const {
    TArray<FActionPlan> Results;

    if (!ValidateActionTable())
    {
        return Results;
    }

    TArray<FName> RowNames = ActionDataTable->GetRowNames();

    for (const FName& RowName : RowNames)
    {
        FActionPlan* Row = ActionDataTable->FindRow<FActionPlan>(RowName, TEXT("GetAllActions"));
        if (Row)
        {
            Results.Add(*Row);
        }
    }

    return Results;
};