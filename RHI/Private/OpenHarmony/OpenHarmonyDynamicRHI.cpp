// Copyright Epic Games, Inc. All Rights Reserved.

#include "RHI.h"
#include "Modules/ModuleManager.h"
#include "OpenHarmony/OpenHarmonyApplication.h"

FDynamicRHI* PlatformCreateDynamicRHI()
{
	FDynamicRHI* DynamicRHI = NULL;

	// Load the dynamic RHI module.
	IDynamicRHIModule* DynamicRHIModule = NULL;
	ERHIFeatureLevel::Type RequestedFeatureLevel = ERHIFeatureLevel::ES3_1;
	FString GraphicsRHI;


	// Vulkan is required, release the EGL created by FAndroidAppEntry::PlatformInit.

	/*
	FOpenHarmonyAppEntry::ReleaseEGL();
	DynamicRHIModule = &FModuleManager::LoadModuleChecked<IDynamicRHIModule>(TEXT("VulkanRHI"));
	GraphicsRHI = TEXT("Vulkan");
	*/
	/*
	DynamicRHIModule = &FModuleManager::LoadModuleChecked<IDynamicRHIModule>(TEXT("OpenGLDrv"));
	GraphicsRHI = TEXT("OpenGL");
	*/

	if (FPlatformMisc::ShouldUseVulkan() || FPlatformMisc::ShouldUseDesktopVulkan())
	{
		// Vulkan is required, release the EGL created by FAndroidAppEntry::PlatformInit.
		FOpenHarmonyAppEntry::ReleaseEGL();

		DynamicRHIModule = &FModuleManager::LoadModuleChecked<IDynamicRHIModule>(TEXT("VulkanRHI"));
		if (!DynamicRHIModule->IsSupported())
		{
			DynamicRHIModule = &FModuleManager::LoadModuleChecked<IDynamicRHIModule>(TEXT("OpenGLDrv"));
			GraphicsRHI = TEXT("OpenGL");
		}
		else
		{
			RequestedFeatureLevel = FPlatformMisc::ShouldUseDesktopVulkan() ? ERHIFeatureLevel::SM5 : ERHIFeatureLevel::ES3_1;
			GraphicsRHI = TEXT("Vulkan");
		}
	}
	else
	{
		DynamicRHIModule = &FModuleManager::LoadModuleChecked<IDynamicRHIModule>(TEXT("OpenGLDrv"));
		GraphicsRHI = TEXT("OpenGL");
	}


	//UE_LOG(LogTemp, Error, TEXT("HMSDynamicRHIModuleSupported %s"), DynamicRHIModule->IsSupported());
	//UE_LOG(LogTemp, Error, TEXT("HMSDynamicRHIModuleNULL %s"), DynamicRHIModule == NULL);

	if (!DynamicRHIModule->IsSupported())
	{
		//	FMessageDialog::Open(EAppMsgType::Ok, TEXT("OpenGL 3.2 is required to run the engine."));
		FPlatformMisc::RequestExit(1);
		DynamicRHIModule = NULL;
	}

	if (DynamicRHIModule)
	{
		FApp::SetGraphicsRHI(GraphicsRHI);
		// Create the dynamic RHI.
		DynamicRHI = DynamicRHIModule->CreateRHI(RequestedFeatureLevel);
	}

	FPlatformMisc::UnlockOpenHarmonyWindow();

	return DynamicRHI;
}
