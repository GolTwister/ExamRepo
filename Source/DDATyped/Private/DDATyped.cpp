#include "DDATyped.h"

#define LOCTEXT_NAMESPACE "FDDATypedModule"

void FDDATypedModule::StartupModule()
{
    WBPHandler = FWorldDelegates::OnWorldInitializedActors.AddLambda(
        [](const UWorld::FActorsInitializedParams& Params)
        {
            UWorld* World = Params.World;
            if (World && World->IsGameWorld())
            {
                DDA_LOG(TEXT("Plugin: level started"));
                AActor* FoundManager = UGameplayStatics::GetActorOfClass(World, AMAPEK_Manager::StaticClass());
                if (FoundManager) {
                    DDA_LOG(TEXT("Plugin: Manager found, starting MAPEK"));
                    auto GameInstance = World->GetGameInstance();
                    UMAPEK_Subsystem* MAPEKSubsystem = GameInstance->GetSubsystem<UMAPEK_Subsystem>();
                    if (MAPEKSubsystem)
                    {
                        MAPEKSubsystem->StartMAPECycle();
                    }
                    else {
                        DDA_ERROR(TEXT("Plugin: No MAPEK subsystem found in GameInstanceSubsystems"));
                    }
                }
                else {
                    DDA_WARNING(TEXT("Plugin: No MAPEK_Manager found at the level"));
                }
            }
        }
    );
}

void FDDATypedModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDDATypedModule, DDATyped)