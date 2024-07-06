// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ClassicLauncher : ModuleRules
{
	public ClassicLauncher(ReadOnlyTargetRules Target) : base(Target)
    {

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Directories dependencies
		RuntimeDependencies.Add("$(ProjectDir)/config/configsys.xml"); 
		RuntimeDependencies.Add("$(ProjectDir)/config/config.xml");
		RuntimeDependencies.Add("$(ProjectDir)/musics/*");

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" , "UMG" , "MediaAssets", "TextImageBox" });

		// third plugins  
		PrivateDependencyModuleNames.AddRange(new string[] { "EasyXMLParser" , "RuntimeAudioImporter" });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		PrivateDependencyModuleNames.AddRange(new string[] { "RHI", "ImageWrapper", "RenderCore", "ImageCore", "FreeImage", "HTTP" , "Json" , "XmlParser" });	

		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
