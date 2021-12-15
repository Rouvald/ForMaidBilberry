// For Maid Bilberry Game. All Rights Recerved


#include "Menu/UI/FMBLevelItemWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UFMBLevelItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if(SelectLevelButton)
    {
        SelectLevelButton->OnClicked.AddDynamic(this, &UFMBLevelItemWidget::OnLevelItemClicked);
    }
}

void UFMBLevelItemWidget::OnLevelItemClicked()
{
    OnLevelSelected.Broadcast(LevelData);
}

void UFMBLevelItemWidget::SetLevelData(const FLevelData& Data)
{
    LevelData = Data;

    if(LevelNameTextBlock)
    {
        LevelNameTextBlock->SetText(FText::FromName(LevelData.LevelDisplayName));
    }
    if(LevelImage)
    {
        LevelImage->SetBrushFromTexture(LevelData.LevelImage);
    }
}

void UFMBLevelItemWidget::IsLevelSelected( bool IsSelected)
{
    if(LevelFrameImage)
    {
        LevelFrameImage->SetVisibility(IsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}