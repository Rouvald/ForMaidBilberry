// For Maid Bilberry Game. All Rights Recerved

#include "UI/FMBKeyboardInfoWidget.h"
#include "FMBPlayerController.h"
//#include "Components/TextBlock.h"
#include "Components/Overlay.h"

void UFMBKeyboardInfoWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (KeyboardInfoOverlay)
    {
        KeyboardInfoOverlay->SetVisibility(ESlateVisibility::Visible);
    }
    if (KeyboardInfoVisibilityOverlay)
    {
        KeyboardInfoVisibilityOverlay->SetVisibility(ESlateVisibility::Visible);
    }
}

void UFMBKeyboardInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    ChangeKeyboardInfoVisibility();
}

void UFMBKeyboardInfoWidget::ChangeKeyboardInfoVisibility() const
{
    const auto PlayerController = GetPlayerController();
    if (!PlayerController) return;

    if (KeyboardInfoOverlay)
    {
        KeyboardInfoOverlay->SetVisibility(
            PlayerController->GetIsKeyboardInfoVisible() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

bool UFMBKeyboardInfoWidget::GetIsKeyboardInfoVisible() const
{
    const auto PlayerController = GetPlayerController();
    if (!PlayerController) return false;

    return PlayerController->GetIsKeyboardInfoVisible();
}

AFMBPlayerController* UFMBKeyboardInfoWidget::GetPlayerController() const
{
    if (!GetWorld()) return nullptr;
    return Cast<AFMBPlayerController>(GetWorld()->GetFirstPlayerController());
}
