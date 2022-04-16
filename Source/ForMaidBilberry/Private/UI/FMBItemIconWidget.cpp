// For Maid Bilberry Game. All Rights Reserved

#include "UI/FMBItemIconWidget.h"
#include "FMBUtils.h"
#include "Components/TextBlock.h"

void UFMBItemIconWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (SelectedItemFrame)
    {
        SelectedItemFrame->SetVisibility(ESlateVisibility::Collapsed);
    }
    if (ItemImage)
    {
        ItemImage->SetVisibility(ESlateVisibility::Collapsed);
    }
    if (ItemCount)
    {
        ItemCount->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UFMBItemIconWidget::SetItemImage(UTexture2D* WeaponImage) const
{
    if (ItemImage)
    {
        if (WeaponImage)
        {
            ItemImage->SetVisibility(ESlateVisibility::Visible);
            ItemImage->SetBrushFromTexture(WeaponImage);
        }
        else
        {
            ItemImage->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

void UFMBItemIconWidget::SetItemCount(const int32 NewItemCount) const
{
    if (ItemCount)
    {
        if (NewItemCount > 1)
        {
            ItemCount->SetVisibility(ESlateVisibility::Visible);
            ItemCount->SetText(FMBUtils::TextFromInt(NewItemCount));
        }
        else
        {
            ItemCount->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

void UFMBItemIconWidget::SetVisibleItemImage(const bool bIsVisible) const
{
    if (ItemImage)
    {
        ItemImage->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
}

void UFMBItemIconWidget::ItemIsSelected(const bool bIsSelected) const
{
    if (SelectedItemFrame)
    {
        SelectedItemFrame->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
}