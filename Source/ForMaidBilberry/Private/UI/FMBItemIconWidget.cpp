// For Maid Bilberry Game. All Rights Reserved

#include "UI/FMBItemIconWidget.h"
#include "FMBUtils.h"
#include "Components/SizeBox.h"
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
    if (ItemCountTextBlock)
    {
        ItemCountTextBlock->SetVisibility(ESlateVisibility::Collapsed);
    }
    SetBigItemIconSize();
}

void UFMBItemIconWidget::SetItemImage(UTexture2D* ItemTexture2D) const
{
    if (ItemImage)
    {
        if (ItemTexture2D)
        {
            ItemImage->SetVisibility(ESlateVisibility::Visible);
            ItemImage->SetBrushFromTexture(ItemTexture2D);
        }
        else
        {
            ItemImage->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

void UFMBItemIconWidget::SetItemCount(const int32 NewItemCount) const
{
    if (ItemCountTextBlock)
    {
        if (NewItemCount > 1)
        {
            ItemCountTextBlock->SetVisibility(ESlateVisibility::Visible);
            ItemCountTextBlock->SetText(FMBUtils::TextFromInt(NewItemCount));
        }
        else
        {
            ItemCountTextBlock->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

void UFMBItemIconWidget::SetBigItemIconSize() const
{
    if (ItemIconSize)
    {
        ItemIconSize->SetWidthOverride(BigItemIconSize.X);
        ItemIconSize->SetHeightOverride(BigItemIconSize.Y);
    }
}

void UFMBItemIconWidget::SetSmallItemIconSize() const
{
    if (ItemIconSize)
    {
        ItemIconSize->SetWidthOverride(SmallItemIconSize.X);
        ItemIconSize->SetHeightOverride(SmallItemIconSize.Y);
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