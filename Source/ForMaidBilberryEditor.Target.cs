// For Maid Bilberry Game. All Rights Reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class ForMaidBilberryEditorTarget : TargetRules
{
    public ForMaidBilberryEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;

        ExtraModuleNames.AddRange(new string[] { "ForMaidBilberry" });
    }
}
