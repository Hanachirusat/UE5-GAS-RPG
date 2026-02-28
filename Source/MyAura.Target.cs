

using UnrealBuildTool;
using System.Collections.Generic;

public class MyAuraTarget : TargetRules
{
	public MyAuraTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "MyAura" } );
	}
}
