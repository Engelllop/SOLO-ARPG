using UnrealBuildTool;
using System.Collections.Generic;

public class SOLOEditorTarget : TargetRules
{
	public SOLOEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		// Incluir SOLOCore explÃ­citamente: sin esto, Python/Editor puede no registrar
		// UClass de Core y load_class("/Script/SOLOCore....") falla.
		ExtraModuleNames.AddRange(new string[] { "SOLO", "SOLOCore" });
	}
}
