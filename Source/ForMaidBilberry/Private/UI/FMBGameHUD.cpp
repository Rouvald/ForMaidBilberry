// For Maid Bilberry Game. All Rights Recerved

#include "UI/FMBGameHUD.h"
#include "Engine/Canvas.h"
#include "FMBGameModeBase.h"
#include "UI/FMBBaseWidget.h"
#include "Components/Image.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBGameHUD, All, All)

void AFMBGameHUD::BeginPlay()
{
    Super::BeginPlay();

    GameWidgets.Add(EGameState::EGS_InProgress, CreateWidget<UFMBBaseWidget>(GetWorld(), PLayerHUDWidgetClass));
    GameWidgets.Add(EGameState::EGS_Pause, CreateWidget<UFMBBaseWidget>(GetWorld(), GamePauseWidgetClass));
    GameWidgets.Add(EGameState::EGS_GameOver, CreateWidget<UFMBBaseWidget>(GetWorld(), GameOverWidgetClass));

    for (const auto GameWidgetPair : GameWidgets)
    {
        const auto GameWidget = GameWidgetPair.Value;
        if (!GameWidget) continue;

        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (GetWorld())
    {
        if (const auto GameMode = Cast<AFMBGameModeBase>(GetWorld()->GetAuthGameMode()))
        {
            GameMode->OnMatchStateChange.AddUObject(this, &AFMBGameHUD::OnMatchStateChange);
        }
    }
    // SetCharacter();
}

/*void AFMBGameHUD::SetCharacter()
{
    if (GetOwner())
    {
        if(const auto FMBController = Cast<AController>(GetOwner()))
        {
            Character = Cast<AFMBPlayerCharacter>(FMBController->GetCharacter());
        }
    }
}*/

void AFMBGameHUD::OnMatchStateChange(EGameState State)
{
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    if (GameWidgets.Contains(State))
    {
        CurrentWidget = GameWidgets[State];
    }
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
        CurrentWidget->Show();
    }
    // UE_LOG(LogFMBGameHUD, Display, TEXT("Match State: %s"), *UEnum::GetValueAsString(State));
}

/*
 *void AFMBGameHUD::DrawHUD()
{
    if (Character && Character->GetIsFPP())
    {
        DrawCrossHair();
    }
}

void AFMBGameHUD::DrawCrossHair()
{
    if (!DotCrossHairTexture) return;
    UImage* DotCrossHair{nullptr};
    DotCrossHair->SetBrushFromTexture(DotCrossHairTexture);

    const FVector2D ViewportCenterLocation = FVector2D{Canvas->SizeX / 2.0f, Canvas->SizeY / 2.0f};

    const FVector2D TextureCenterLocation = FVector2D{DotCrossHairTexture->GetSizeX() * DotCrossHairTextureScale / 2.0f,
                                                      DotCrossHairTexture->GetSizeY() * DotCrossHairTextureScale / 2.0f};

    DrawTextureSimple
        (
            DotCrossHairTexture,                                //
            ViewportCenterLocation.X - TextureCenterLocation.X, //
            ViewportCenterLocation.Y - TextureCenterLocation.Y, //
            DotCrossHairTextureScale
            );
}
*/
