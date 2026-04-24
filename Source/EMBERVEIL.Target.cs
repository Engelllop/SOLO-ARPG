using UnrealBuildTool;
using System.Collections.Generic;

public class EMBERVEILTarget : TargetRules
{
	public EMBERVEILTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange(new string[] { "EMBERVEIL" });
	}
}
