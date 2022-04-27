// For Maid Bilberry Game. All Rights Reserved

#include "Menu/FMBMenuGameModeBase.h"
#include "FMBBaseCharacter.h"
#include "FMBMenuHUD.h"
#include "Menu/FMBMenuPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBMenuGameModeBase, All, All)

AFMBMenuGameModeBase::AFMBMenuGameModeBase()
{
    PlayerControllerClass = AFMBMenuPlayerController::StaticClass();
    HUDClass = AFMBMenuHUD::StaticClass();
}

void AFMBMenuGameModeBase::StartPlay()
{
    Super::StartPlay();

    SetMenuState(EMenuState::EMS_MainMenu);
}

void AFMBMenuGameModeBase::SetMenuState(EMenuState State)
{
    if (CurrentMenuState == State) return;

    CurrentMenuState = State;
    OnMenuStateChange.Broadcast(CurrentMenuState);
}

/*void AFMBMenuGameModeBase::SetCharacterSkeletalMesh(USkeletalMesh* NewSkeletalMesh)
{
    if (!GetWorld()) return;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;

        const auto Character = Cast<AFMBBaseCharacter>(Controller->GetCharacter());
        if (!Character) continue;

        UE_LOG(LogFMBMenuGameModeBase, Error, TEXT("Character name: %s"), *Character->GetName());
        Character->SetTeamSkeletalMesh(NewSkeletalMesh);
    }
}*/
