using UnrealBuildTool;

public class SOLOCore : ModuleRules
{
	public SOLOCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"EnhancedInput",
			"InputCore",
			"NetCore",
			"AIModule",
			"NavigationSystem",
			"UMG",
			"CommonUI",
			"Slate",
			"SlateCore",
			"Niagara"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"OnlineSubsystem",
			"OnlineSubsystemUtils"
		});

		// Necesario para GAS en builds de shipping
		PublicDefinitions.Add("WITH_GAMEPLAY_DEBUGGER=1");
	}
}
