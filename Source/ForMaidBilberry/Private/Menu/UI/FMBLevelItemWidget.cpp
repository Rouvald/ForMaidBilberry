// For Maid Bilberry Game. All Rights Recerved

#include "Menu/UI/FMBLevelItemWidget.h"
#include "FMBGameInstance.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void UFMBLevelItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (SelectLevelButton)
    {
        SelectLevelButton->OnClicked.AddDynamic(this, &UFMBLevelItemWidget::OnLevelItemClicked);
        /*SelectLevelButton->OnHovered.AddDynamic(this, &UFMBLevelItemWidget::OnLevelItemHovered);
        SelectLevelButton->OnUnhovered.AddDynamic(this, &UFMBLevelItemWidget::OnLevelItemUnHovered);*/
    }
    if (LevelIconSizeBox)
    {
        LevelIconSizeBox->WidthOverride = NormalSizeBox;
        LevelIconSizeBox->HeightOverride = NormalSizeBox;
    }
    if (LevelNameTextBlock)
    {
        LevelNameTextBlock->SetVisibility(ESlateVisibility::Hidden);
    }
    /*if (LevelDescriptionTextBlock)
    {
        LevelDescriptionTextBlock->SetVisibility(ESlateVisibility::Hidden);
    }*/
}

void UFMBLevelItemWidget::OnLevelItemClicked()
{
    OnLevelSelected.Broadcast(LevelData);
}

void UFMBLevelItemWidget::SetLevelData(const FLevelData& Data)
{
    LevelData = Data;

    if (LevelNameTextBlock)
    {
        LevelNameTextBlock->SetText(FText::FromName(LevelData.LevelDisplayName));
    }
    /*if (LevelDescriptionTextBlock)
    {
        LevelDescriptionTextBlock->SetText(FText::Format(FTextFormat{}, LevelData.LevelDescriptionText));
    }*/
    if (LevelImage)
    {
        LevelImage->SetBrushFromTexture(LevelData.LevelImage);
    }
}

void UFMBLevelItemWidget::IsLevelSelected(bool IsSelected)
{
    if (LevelIconSizeBox)
    {
        LevelIconSizeBox->SetWidthOverride(IsSelected ? SelectedSizeBox : NormalSizeBox);
        LevelIconSizeBox->SetHeightOverride(IsSelected ? SelectedSizeBox : NormalSizeBox);
    }
    if (LevelNameTextBlock)
    {
        LevelNameTextBlock->SetVisibility(IsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
    /*if (LevelDescriptionTextBlock)
    {
        LevelDescriptionTextBlock->SetVisibility(IsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }*/
}

/*void UFMBLevelItemWidget::OnLevelItemHovered()
{
    if (LevelNameTextBlock)
    {
        LevelNameTextBlock->SetVisibility(ESlateVisibility::Visible);
    }
    if (LevelDescriptionTextBlock)
    {
        LevelDescriptionTextBlock->SetVisibility(ESlateVisibility::Visible);
    }
}

void UFMBLevelItemWidget::OnLevelItemUnHovered()
{
    if (LevelNameTextBlock)
    {
        LevelNameTextBlock->SetVisibility(ESlateVisibility::Hidden);
    }
    if (LevelDescriptionTextBlock)
    {
        LevelDescriptionTextBlock->SetVisibility(ESlateVisibility::Hidden);
    }
}*/
