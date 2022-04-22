// For Maid Bilberry Game. All Rights Reserved

#include "UI/FMBItemIconWidget.h"
#include "FMBUtils.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBItemIconWidget, All, All);

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

    ItemIconSizeMap.Add(EItemType::EIT_Weapon, WeaponItemIconSize);
    ItemIconSizeMap.Add(EItemType::EIT_PickUp, PickUpItemIconSize);
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

void UFMBItemIconWidget::SetItemIconSize(EItemType ItemType) const
{
    if (ItemIconSize)
    {
        if (ItemIconSizeMap.Contains(ItemType))
        {
            ItemIconSize->SetWidthOverride(ItemIconSizeMap[ItemType]);
            ItemIconSize->SetHeightOverride(ItemIconSizeMap[ItemType]);
        }
    }
}

void UFMBItemIconWidget::SetVisibleItemImage(const bool bIsVisible) const
{
    if (ItemImage)
    {
        ItemImage->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
        // UE_LOG(LogFMBItemIconWidget, Error, TEXT("Name: %s, %s"), *GetName(), *UEnum::GetValueAsString(ItemImage->GetVisibility()));
    }
}

void UFMBItemIconWidget::SetItemIsSelected(const bool bIsVisible) const
{
    if (SelectedItemFrame)
    {
        SelectedItemFrame->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
}

void UFMBItemIconWidget::SetItemCountVisible(const bool bIsVisible) const
{
    if (ItemCountTextBlock)
    {
        ItemCountTextBlock->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
}
