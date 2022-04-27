// For Maid Bilberry Game. All Rights Reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class ForMaidBilberryTarget : TargetRules
{
    public ForMaidBilberryTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;

        ExtraModuleNames.AddRange(new string[] { "ForMaidBilberry" });
    }
}
