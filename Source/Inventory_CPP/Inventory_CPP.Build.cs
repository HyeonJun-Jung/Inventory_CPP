// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Inventory_CPP : ModuleRules
{
	public Inventory_CPP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "SlateCore" });

        PrivateIncludePaths.Add("Inventory_CPP");
    }
}
