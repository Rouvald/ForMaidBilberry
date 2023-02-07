@echo off

rem Engine params
set enginePath_4.27=D:\UE_4_27_2
set enginePath_5.0=D:\UE_5_1_0

set UBTRelativePath4=Engine\Binaries\DotNET\UnrealBuildTool.exe
set UBTRelativePath5=Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe
set versionSelector=D:\Program_Files_86\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe

rem Engine version for packaging
set runUATPath=%enginePath_4.27%\Engine\Build\BatchFiles\RunUAT.bat

rem Project params
set projectRoot=c:\UE4_Projects\ForMaidBilberry
set projectPureName=ForMaidBilberry
set projectName=ForMaidBilberry.uproject
set projectPath=%projectRoot%\%projectName%

rem Build params
set platform=Win64
set configuration=Development
set archievePath=%projectRoot%\Build

rem Other params
set sourceCodePath=%projectRoot%\Source
set dirsToRemove=Intermediate DerivedDataCache Saved Binaries .vs Build
set filesToRemove=*.sln

rem Target params
set COPYRIGHT_LINE=// For Maid Bilberry Game. All Rights Reserved