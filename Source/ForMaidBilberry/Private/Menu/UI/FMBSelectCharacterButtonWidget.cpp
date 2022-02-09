// For Maid Bilberry Game. All Rights Recerved

#include "Menu/UI/FMBSelectCharacterButtonWidget.h"
#include "FMBMenuGameModeBase.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBSelectCharacterButtonWidget, All, All)

/*
void UFMBSelectCharacterButtonWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (SelectCharacterMeshButton)
    {
        SelectCharacterMeshButton->OnClicked.AddDynamic(this, &UFMBSelectCharacterButtonWidget::OnCharacterSelectButtonClicked);
    }
}

void UFMBSelectCharacterButtonWidget::OnCharacterSelectButtonClicked()
{
    OnCharacterMeshSelected.Broadcast(CharacterData);
}

void UFMBSelectCharacterButtonWidget::SetCharacterData(const FCharacterData& Data)
{
    CharacterData = Data;

    if (CharacterNameTextBlock)
    {
        CharacterNameTextBlock->SetText(FText::FromName(CharacterData.CharacterName));
    }
}

void UFMBSelectCharacterButtonWidget::IsCharacterMeshSelected(bool IsSelected)
{
    if (CharacterNameSizeBox)
    {
        CharacterNameSizeBox->SetWidthOverride(IsSelected ? SelectedCharacterNameSizeBox.X : NormalCharacterNameSizeBox.X);
        CharacterNameSizeBox->SetHeightOverride(IsSelected ? SelectedCharacterNameSizeBox.Y : NormalCharacterNameSizeBox.Y);
    }
    /*if (!GetWorld()) return;

    const auto MenuGameMode = Cast<AFMBMenuGameModeBase>(GetWorld()->GetAuthGameMode());
    if (MenuGameMode)
    {
    UE_LOG(LogFMBSelectCharacterButtonWidget, Error, TEXT("we have menu gamemode"));
    MenuGameMode->SetCharacterSkeletalMesh(CharacterData.CharacterMesh);
    }#1#
}*/
