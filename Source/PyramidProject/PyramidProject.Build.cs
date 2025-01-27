// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PyramidProject : ModuleRules
{
	public PyramidProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject",
			"Engine",
			"InputCore",
			"HeadMountedDisplay",
			"UMG",
            "MultiplayerSession",
            "Json",
            "JsonUtilities"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { 
			"Slate",
			"SlateCore"
        });
    }
}
