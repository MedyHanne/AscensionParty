// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class AscensionParty : ModuleRules
{
	public AscensionParty(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "OnlineSubsystem", "OnlineSubsystemUtils", "Networking", "EnhancedInput","UMG", "CommonInput", 
    "OnlineServicesInterface" });

        RuntimeDependencies.Add(
     "$(TargetOutputDir)/steam_appid.txt",
     "$(ProjectDir)/steam_appid.txt",
     StagedFileType.NonUFS
 );
        RuntimeDependencies.Add(
    "$(TargetOutputDir)/steam_api64.dll",
    "$(EngineDir)/Binaries/ThirdParty/Steamworks/Steamv157/Win64/steam_api64.dll",
    StagedFileType.NonUFS
);

        PrivateDependencyModuleNames.AddRange(new string[] {  });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
