// For Maid Bilberry Game. All Rights Reserved

#include "UI/FMBItemInfoWidget.h"

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