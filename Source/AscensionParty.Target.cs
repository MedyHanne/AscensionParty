// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class AscensionPartyTarget : TargetRules
{
	public AscensionPartyTarget(TargetInfo Target) : base(Target)
	{
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
        bOverrideBuildEnvironment = true; // ← remplace BuildEnvironment = Unique
        ExtraModuleNames.AddRange(new string[] { "AscensionParty" });
        
        bUseLoggingInShipping = true;
    }
}
