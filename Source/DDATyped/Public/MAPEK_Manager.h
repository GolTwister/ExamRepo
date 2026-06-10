#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MAPEK_SymptomLibrary.h"
#include "MAPEK_ActionPull.h"
#include "Engine/DataTable.h"
#include "MAPEK_Manager.generated.h"

UCLASS()
class DDATYPED_API AMAPEK_Manager : public AActor
{
	GENERATED_BODY()
	
public:	
	AMAPEK_Manager();

    // Choose DataTable with symptoms definition
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MAPEK Configuration")
    UDataTable* SymptomDataTable;

    // Check up for structur of table
    UFUNCTION(BlueprintCallable, Category = "MAPEK Configuration")
    bool ValidateSymptomTable() const;

    // Geter Symptoms
    UFUNCTION(BlueprintCallable, Category = "MAPEK Configuration")
    TArray<FSymptomDefinition> GetAllSymptoms() const;


    // Choose DataTable with action pull
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MAPEK Configuration")
    UDataTable* ActionDataTable;

    // Check up for structur of table
    UFUNCTION(BlueprintCallable, Category = "MAPEK Configuration")
    bool ValidateActionTable() const;

    // Geter Actions
    UFUNCTION(BlueprintCallable, Category = "MAPEK Configuration")
    TArray<FActionPlan> GetAllActions() const;

protected:
	virtual void BeginPlay() override;
    bool bIsDataTableValid = false;

public:	
	virtual void Tick(float DeltaTime) override;
};
