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
    }
    if (LevelIconSizeBox)
    {
        LevelIconSizeBox->WidthOverride = NormalSizeBox;
        LevelIconSizeBox->HeightOverride = NormalSizeBox;
    }
    if(LevelDescriptionTextBlock)
    {
        LevelDescriptionTextBlock->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UFMBLevelItemWidget::OnLevelItemClicked()
{
    OnLevelSelected.Broadcast(LevelData);
}

void UFMBLevelItemWidget::SetLevelData(const FLevelData& Data)
{
    LevelData = Data;

    if (LevelDescriptionTextBlock)
    {
        LevelDescriptionTextBlock->SetText(FText::FromName(LevelData.LevelDescriptionName));
    }
    if (LevelImage)
    {
        LevelImage->SetBrushFromTexture(LevelData.LevelImage);
    }
}

void UFMBLevelItemWidget::IsLevelSelected(bool IsSelected)
{
    /*if (LevelFrameImage)
    {
        LevelFrameImage->SetVisibility(IsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }*/
    if (LevelIconSizeBox)
    {
        LevelIconSizeBox->SetWidthOverride(IsSelected ? SelectedSizeBox : NormalSizeBox);
        LevelIconSizeBox->SetHeightOverride(IsSelected ? SelectedSizeBox : NormalSizeBox);
    }
    if(LevelDescriptionTextBlock)
    {
        LevelDescriptionTextBlock->SetVisibility(IsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}
