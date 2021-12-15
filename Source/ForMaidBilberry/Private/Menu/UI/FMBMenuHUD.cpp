// For Maid Bilberry Game. All Rights Recerved


#include "Menu/UI/FMBMenuHUD.h"
#include "Blueprint/UserWidget.h"

void AFMBMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    if(MenuWidgetClass)
    {
        const auto MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
        if(MenuWidget)
        {
            MenuWidget->AddToViewport();
        }
    }
}
