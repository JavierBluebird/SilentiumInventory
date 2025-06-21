// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SilentiumInventory : ModuleRules
{
	public SilentiumInventory(ReadOnlyTargetRules Target) : base(Target)
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
			"UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"SilentiumInventory",
			"SilentiumInventory/Variant_Platforming",
			"SilentiumInventory/Variant_Combat",
			"SilentiumInventory/Variant_Combat/AI",
			"SilentiumInventory/Variant_SideScrolling",
			"SilentiumInventory/Variant_SideScrolling/Gameplay",
			"SilentiumInventory/Variant_SideScrolling/AI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
