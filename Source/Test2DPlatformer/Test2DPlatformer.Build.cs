// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Test2DPlatformer : ModuleRules
{
	public Test2DPlatformer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "AIModule", "Core", "CoreUObject", "Engine", "GameplayTasks", "InputCore", "Paper2D", "UMG" });

		// The following is needed for the online leaderboard
		PrivateDependencyModuleNames.AddRange(new string[]{"HTTP", "Json", "JsonUtilities"});
		
		// Needed by UMG
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		//PrivateDependencyModuleNames.AddRange(new string[] {  });

		// tanis - start faster compile time for small projects
		//MinFilesUsingPrecompiledHeaderOverride = 1;
 		//bFasterWithoutUnity = true;
 		// tanis - end

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");
		// if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
		// {
		//		if (UEBuildConfiguration.bCompileSteamOSS == true)
		//		{
		//			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
		//		}
		// }
	}
}
