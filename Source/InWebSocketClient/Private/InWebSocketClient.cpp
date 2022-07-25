// Copyright Epic Games, Inc. All Rights Reserved.

#include "InWebSocketClient.h"
#include "WebSocketsModule.h"

DEFINE_LOG_CATEGORY(LogInWebSocketClientModule);

#define LOCTEXT_NAMESPACE "FInWebSocketClientModule"

void FInWebSocketClientModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	const FName WebSocketsModuleName = TEXT("WebSockets");

	if (!FModuleManager::Get().IsModuleLoaded(WebSocketsModuleName))
	{
		FWebSocketsModule& Module = FModuleManager::LoadModuleChecked<FWebSocketsModule>(WebSocketsModuleName);
	}
}

void FInWebSocketClientModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FInWebSocketClientModule, InWebSocketClient)