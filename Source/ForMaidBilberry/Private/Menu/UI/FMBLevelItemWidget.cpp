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
        SelectLevelButton->OnHovered.AddDynamic(this, &UFMBLevelItemWidget::OnLevelItemHovered);
        SelectLevelButton->OnUnhovered.AddDynamic(this, &UFMBLevelItemWidget::OnLevelItemUnHovered);
    }
    if (LevelIconSizeBox)
    {
        LevelIconSizeBox->SetWidthOverride(NormalIconSizeBox);
        LevelIconSizeBox->SetHeightOverride(NormalIconSizeBox);
    }
    if (LevelFrameSizeBox)
    {
        LevelFrameSizeBox->SetWidthOverride(NormalFrameSizeBox);
        LevelFrameSizeBox->SetHeightOverride(NormalFrameSizeBox);
    }
    if (LevelNameTextBlock)
    {
        LevelNameTextBlock->SetVisibility(ESlateVisibility::Hidden);
    }
    if (LevelDescriptionTextBlock)
    {
        LevelDescriptionTextBlock->SetVisibility(ESlateVisibility::Hidden);
    }
    if (LevelHoveredImage)
    {
        LevelHoveredImage->SetRenderOpacity(0.0f);
    }
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
    if (LevelDescriptionTextBlock)
    {
        const FString ModeText{TEXT("Mode: ")};
        const FText LevelMode{FText::FromString(ModeText + LevelData.LevelDescriptionName.ToString())};
        LevelDescriptionTextBlock->SetText(LevelMode);
        UE_LOG(LogTemp, Display, TEXT("%s"), *LevelDescriptionTextBlock->GetText().ToString());
    }
    if (LevelImage)
    {
        LevelImage->SetBrushFromTexture(LevelData.LevelImage);
    }
}

void UFMBLevelItemWidget::IsLevelSelected(bool IsSelected)
{
    if (LevelIconSizeBox)
    {
        LevelIconSizeBox->SetWidthOverride(IsSelected ? SelectedIconSizeBox : NormalIconSizeBox);
        LevelIconSizeBox->SetHeightOverride(IsSelected ? SelectedIconSizeBox : NormalIconSizeBox);
    }
    if (LevelFrameSizeBox)
    {
        LevelFrameSizeBox->SetWidthOverride(IsSelected ? SelectedFrameSizeBox : NormalFrameSizeBox);
        LevelFrameSizeBox->SetHeightOverride(IsSelected ? SelectedFrameSizeBox : NormalFrameSizeBox);
    }
    if (LevelNameTextBlock)
    {
        LevelNameTextBlock->SetVisibility(IsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
    if (LevelDescriptionTextBlock)
    {
        LevelDescriptionTextBlock->SetVisibility(IsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void UFMBLevelItemWidget::OnLevelItemHovered()
{
    if (LevelHoveredImage)
    {
        LevelHoveredImage->SetRenderOpacity(LevelItemHoveredRenderOpacity);
    }
}

void UFMBLevelItemWidget::OnLevelItemUnHovered()
{
    if (LevelHoveredImage)
    {
        LevelHoveredImage->SetRenderOpacity(0.0f);
    }
}
