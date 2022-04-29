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
}

void UFMBItemInfoWidget::SetItemName(const FName& Name) const
{
    if (ItemName)
    {
        ItemName->SetText(FText::FromName(Name));
    }
}

/* Fast Attack damage */
void UFMBItemInfoWidget::SetItemProperty(float NewProperty)
{
    Property = NewProperty;
}

FText UFMBItemInfoWidget::GetItemProperty() const
{
    return Property >= 0.0f ? FText::FromString("+" + FString::SanitizeFloat(Property, /*2*/ 0))
                            : FText::FromString(FString::SanitizeFloat(Property, /*2*/ 0));
}

FSlateColor UFMBItemInfoWidget::GetItemPropertyColor() const
{
    return Property >= 0.0f ? FSlateColor{ItemInfoColorGreen} : FSlateColor{ItemInfoColorRed};
}

//

/* Strong Attack damage */
/*void UFMBItemInfoWidget::SetItemStrongAttackDamage(float NewStrongAttackDamage)
{
    StrongAttackDamage = NewStrongAttackDamage;
}
FText UFMBItemInfoWidget::GetItemStrongAttackDamage() const
{
    return FText::FromString(FString::SanitizeFloat(StrongAttackDamage, /*2#1#0));
}*/
//

/* Health amount */
/*void UFMBItemInfoWidget::SetItemHealthAmount(float NewHealthAmount)
{
    HealthAmount = NewHealthAmount;
}

FText UFMBItemInfoWidget::GetItemHealthAmount() const
{
    return FText::FromString("+" + FString::SanitizeFloat(HealthAmount, /*2#1# 0));
}*/

//

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
