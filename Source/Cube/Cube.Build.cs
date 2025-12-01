// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Cube : ModuleRules
{
	public Cube(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "NavigationSystem",
            "UMG",
            "AIModule",
            "GeometryCollectionEngine"

        });


        PrivateDependencyModuleNames.AddRange(new string[] {
            "Slate",       // ƒл€ Slate UI
			"SlateCore"    // ядро Slate UI
		
		
		
		
		});

        PublicIncludePaths.AddRange(new string[] {
            "Cube/Characters",
            "Cube/Fight",
            "Cube/Weapons",
            "Cube/MainMenu"
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
