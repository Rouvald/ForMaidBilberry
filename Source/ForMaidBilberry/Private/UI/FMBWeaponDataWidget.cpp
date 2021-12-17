// For Maid Bilberry Game. All Rights Recerved


#include "UI/FMBWeaponDataWidget.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBWeaponDataWidget, All, All)

void UFMBWeaponDataWidget::SetWeaponUIData(const FWeaponUIData& Data)
{
    WeaponUIData = Data;
    
    if (WeaponUIImage)
    {
        WeaponUIImage->SetBrushFromTexture(WeaponUIData.WeaponIcon);
    }
    if(WeaponUIDataSizeBox)
    {
        WeaponUIDataSizeBox->SetWidthOverride(NormalSizeBox);
        WeaponUIDataSizeBox->SetHeightOverride(NormalSizeBox);
    }
}

void UFMBWeaponDataWidget::WeaponIsSelected(bool IsSelected)
{
    UE_LOG(LogFMBWeaponDataWidget, Error, TEXT("weapon is selected"));
    if (WeaponUIDataSizeBox)
    {
        WeaponUIDataSizeBox->SetWidthOverride(IsSelected ? SelectedSizeBox : NormalSizeBox);
        WeaponUIDataSizeBox->SetHeightOverride(IsSelected ? SelectedSizeBox : NormalSizeBox);
    }
}
