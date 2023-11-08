// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ClassicLauncher : ModuleRules
{
	public ClassicLauncher(ReadOnlyTargetRules Target) : base(Target)
    {

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// dir dependecys
		RuntimeDependencies.Add("$(ProjectDir)/7zip/*");
		RuntimeDependencies.Add("$(ProjectDir)/config/configsys.xml"); 
		RuntimeDependencies.Add("$(ProjectDir)/config/config.xml");
		RuntimeDependencies.Add("$(ProjectDir)/musics/*");

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" , "UMG" , "MediaAssets" });

		PrivateDependencyModuleNames.AddRange(new string[] { "EasyXMLParser"});
		PrivateDependencyModuleNames.AddRange(new string[] { "UnrealLibretro" });
		//PrivateDependencyModuleNames.AddRange(new string[] { "VictoryBPLibrary" }); 
		PrivateDependencyModuleNames.AddRange(new string[] { "TextImageBox" }); 

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });


		PrivateDependencyModuleNames.AddRange(new string[] { "RHI", "ImageWrapper", "RenderCore", "ImageCore", "FreeImage", "HTTP" });	

		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
