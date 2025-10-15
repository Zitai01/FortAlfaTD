// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FortAlfaTD : ModuleRules
{
	public FortAlfaTD(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"Niagara",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"FortAlfaTD",
			"FortAlfaTD/Variant_Strategy",
			"FortAlfaTD/Variant_Strategy/UI",
			"FortAlfaTD/Variant_TwinStick",
			"FortAlfaTD/Variant_TwinStick/AI",
			"FortAlfaTD/Variant_TwinStick/Gameplay",
			"FortAlfaTD/Variant_TwinStick/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
