// For Maid Bilberry Game. All Rights Recerved

#include "Menu/UI/FMBSelectCharacterWidget.h"
#include "FMBGameInstance.h"
#include "Menu/UI/FMBSelectCharacterButtonWidget.h"
#include "Components/VerticalBox.h"

/*
void UFMBSelectCharacterWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    InitCharacterMesh();
}

void UFMBSelectCharacterWidget::InitCharacterMesh()
{
    const auto GameInstance = GetFMBGameInstance();
    if (!GameInstance) return;

    checkf(GameInstance->GetCharactersData().Num() != 0, TEXT("Characters data is EMPTY"));

    if (!CharacterNamesBox) return;
    CharacterNamesBox->ClearChildren();

    int32 CharacterID = 0;
    for (auto CharacterData : GameInstance->GetCharactersData())
    {
        const auto SelectCharacterButtonWidget = CreateWidget<UFMBSelectCharacterButtonWidget>(this, SelectCharacterButtonWidgetClass);
        if (!SelectCharacterButtonWidget) continue;

        CharacterData.CharacterID = CharacterID;
        SelectCharacterButtonWidget->SetCharacterData(CharacterData);
        SelectCharacterButtonWidget->OnCharacterMeshSelected.AddUObject(this, &UFMBSelectCharacterWidget::OnCharacterMeshSelected);

        CharacterNamesBox->AddChild(SelectCharacterButtonWidget);
        SelectCharacterButtonWidgets.Add(SelectCharacterButtonWidget);

        ++CharacterID;
    }
    /*if (GameInstance->GetStartCharacter().CharacterName.IsNone())
    {
        OnCharacterMeshSelected(GameInstance->GetCharactersData()[0]);
    }
    else
    {
        OnCharacterMeshSelected(GameInstance->GetStartCharacter());
    }#1#
}

void UFMBSelectCharacterWidget::OnCharacterMeshSelected(const FCharacterData& Data)
{
    const auto GameInstance = GetFMBGameInstance();
    if (!GameInstance) return;

    // GameInstance->SetStartCharacter(Data);

    for (const auto SelectCharacterButtonWidget : SelectCharacterButtonWidgets)
    {
        if (SelectCharacterButtonWidget)
        {
            const auto IsSelected = Data.CharacterID == SelectCharacterButtonWidget->GetCharacterData().CharacterID;
            SelectCharacterButtonWidget->IsCharacterMeshSelected(IsSelected);
        }
    }
}
UFMBGameInstance* UFMBSelectCharacterWidget::GetFMBGameInstance() const
{
    if (!GetWorld()) return nullptr;
    return GetWorld()->GetGameInstance<UFMBGameInstance>();
}*/