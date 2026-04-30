// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PipPop : ModuleRules
{
	public PipPop(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"SlateCore",
			"Text3D",
			"OnlineSubsystem",
			"OnlineSubsystemNull",
			"OnlineSubsystemSteam",
			"OnlineSubsystemUtils",
			"MediaAssets",
			"MediaUtils",
			"Niagara"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem" });

		PublicIncludePaths.AddRange(new string[] {
			"PipPop"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
