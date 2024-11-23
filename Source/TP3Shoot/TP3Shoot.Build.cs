// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TP3Shoot : ModuleRules
{
	public TP3Shoot(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "GameplayTasks", "AIModule", "NavigationSystem", "UMG", "Niagara" });
	}
}
