// For Maid Bilberry Game. All Rights Reserved

#include "UI/FMBItemInfoWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBItemInfoWidget, All, All)

void UFMBItemInfoWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ItemName)
    {
        ItemName->SetVisibility(ESlateVisibility::Visible);
    }
    if (ItemDamage)
    {
        ItemDamage->SetVisibility(ESlateVisibility::Visible);
    }
}

void UFMBItemInfoWidget::SetItemName(const FName& Name) const
{
    if (ItemName)
    {
        ItemName->SetText(FText::FromName(Name));
    }
}

void UFMBItemInfoWidget::SetItemDamage(float Damage) const
{
    if (ItemDamage)
    {
        ItemDamage->SetText(FText::FromString(FString::SanitizeFloat(Damage, 2)));
    }
}

void UFMBItemInfoWidget::SetItemImage(UTexture2D* ItemTexture2D) const
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