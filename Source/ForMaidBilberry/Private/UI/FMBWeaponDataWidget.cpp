// For Maid Bilberry Game. All Rights Recerved

#include "UI/FMBWeaponDataWidget.h"
#include "Components/Image.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBWeaponDataWidget, All, All)

void UFMBWeaponDataWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ShadowImage)
    {
        UE_LOG(LogFMBWeaponDataWidget, Error, TEXT("ShadowImage have"));
        ShadowImage->SetRenderOpacity(NormalShadowOpacity);
    }
}

void UFMBWeaponDataWidget::SetWeaponUIData(const FWeaponUIData& Data)
{
    WeaponUIData = Data;

    if (WeaponImage)
    {
        UE_LOG(LogFMBWeaponDataWidget, Error, TEXT("WeaponImage have"));
        WeaponImage->SetBrushFromTexture(Data.WeaponIcon);
    }
}

void UFMBWeaponDataWidget::WeaponIsSelected(bool IsSelected)
{
    UE_LOG(LogFMBWeaponDataWidget, Error, TEXT("weapon is selected"));
    if (ShadowImage)
    {
        ShadowImage->SetRenderOpacity(SelectedShadowOpacity);
    }
}
