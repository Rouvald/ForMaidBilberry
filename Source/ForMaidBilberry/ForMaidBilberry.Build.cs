// For Maid Bilberry Game. All Rights Recerved

using UnrealBuildTool;

public class ForMaidBilberry : ModuleRules
{
    public ForMaidBilberry(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",            //
            "CoreUObject",     //
            "Engine",          //
            "InputCore",       //
            "Niagara",         //
            "PhysicsCore",     //
            "GameplayTasks",   //
            "NavigationSystem" //
        });
        PrivateDependencyModuleNames.AddRange(new string[] {});
        PublicIncludePaths.AddRange(new string[] {
            "ForMaidBilberry/Public/Player",            //
            "ForMaidBilberry/Public/Components",        //
            "ForMaidBilberry/Public/Dev",               //
            "ForMaidBilberry/Public/Weapon",            //
            "ForMaidBilberry/Public/UI",                //
            "ForMaidBilberry/Public/PickUps",           //
            "ForMaidBilberry/Public/Weapon/Components", //
            "ForMaidBilberry/Public/AI",                //
            "ForMaidBilberry/Public/AI/Tasks",          //
            "ForMaidBilberry/Public/AI/Services",       //
            "ForMaidBilberry/Public/Menu",              //
            "ForMaidBilberry/Public/Menu/UI",           //
            "ForMaidBilberry/Public/Sound",             //
            "ForMaidBilberry/Public/Sky",               //
            "ForMaidBilberry/Public/GameModes",         //
            "ForMaidBilberry/Public/Environment"        //
        });
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}