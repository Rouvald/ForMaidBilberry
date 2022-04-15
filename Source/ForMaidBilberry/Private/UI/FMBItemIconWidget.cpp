// For Maid Bilberry Game. All Rights Reserved

#include "UI/FMBItemIconWidget.h"

void UFMBItemIconWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (WeaponUiImage)
    {
        WeaponUiImage->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UFMBItemIconWidget::SetWeaponUiImage(const UTexture2D* WeaponImage) const
{
    if (WeaponUiImage)
    {
    }
}