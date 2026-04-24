using UnrealBuildTool;

public class EMBERVEIL : ModuleRules
{
	public EMBERVEIL(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore",
			"EnhancedInput",
			"EMBERVEILCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate", "SlateCore", "UMG"
		});
	}
}
