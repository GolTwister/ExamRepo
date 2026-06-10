#pragma once

#include "Modules/ModuleManager.h"
#include "Engine/World.h"
#include "Logger.h"
#include "Kismet/GameplayStatics.h"
#include "MAPEK_Manager.h"
#include "MAPEK_Subsystem.h"

class FDDATypedModule : public IModuleInterface
{
protected:
	FDelegateHandle WBPHandler;

public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
