using UnrealBuildTool;
using System.Collections.Generic;

public class EMBERVEILEditorTarget : TargetRules
{
	public EMBERVEILEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		// Incluir EMBERVEILCore explícitamente: sin esto, Python/Editor puede no registrar
		// UClass de Core y load_class("/Script/EMBERVEILCore....") falla.
		ExtraModuleNames.AddRange(new string[] { "EMBERVEIL", "EMBERVEILCore" });
	}
}
