

using UnrealBuildTool;
using System.Collections.Generic;

public class MyAuraEditorTarget : TargetRules
{
	public MyAuraEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "MyAura" } );
	}
}
