// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Hypoxia : ModuleRules
{
	public Hypoxia(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "OnlineSubsystem", "OnlineSubsystemUtils", "Voice" });
        //PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}
