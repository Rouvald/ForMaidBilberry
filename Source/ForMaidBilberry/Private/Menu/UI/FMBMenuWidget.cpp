// For Maid Bilberry Game. All Rights Recerved


#include "Menu/UI/FMBMenuWidget.h"
#include "FMBGameInstance.h"
#include "FMBLevelItemWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBMenuWidget, All, All)

void UFMBMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &UFMBMenuWidget::OnStartGame);
    }

    if (QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &UFMBMenuWidget::OnQuitGame);
    }
    InitLevelItem();
}

void UFMBMenuWidget::InitLevelItem()
{
    const auto GameInstance = GetGameInstance();
    if (!GameInstance) return;

    checkf(GameInstance->GetLevelsData().Num() != 0, TEXT("Levels data is EMPTY"));

    if (!LevelItemsBox) return;
    LevelItemsBox->ClearChildren();

    int32 LevelId = 0;
    for (auto LevelData : GameInstance->GetLevelsData())
    {
        const auto LevelItemWidget = CreateWidget<UFMBLevelItemWidget>(this, LevelItemWidgetClass);
        if (!LevelItemWidget) continue;

        LevelData.LevelID = LevelId;
        LevelItemWidget->SetLevelData(LevelData);
        LevelItemWidget->OnLevelSelected.AddUObject(this, &UFMBMenuWidget::OnLevelSelected);

        LevelItemsBox->AddChild(LevelItemWidget);
        LevelItemWidgets.Add(LevelItemWidget);

        ++LevelId;
    }

    if (GameInstance->GetStartLevel().LevelName.IsNone())
    {
        OnLevelSelected(GameInstance->GetLevelsData()[0]);
    }
    else
    {
        OnLevelSelected(GameInstance->GetStartLevel());
    }
}

void UFMBMenuWidget::OnLevelSelected(const FLevelData& Data)
{
    const auto GameInstance = GetGameInstance();
    if (!GameInstance) return;

    GameInstance->SetStartLevel(Data);

    for (const auto LevelItemWidget : LevelItemWidgets)
    {
        if (LevelItemWidget)
        {
            const auto IsSelected = Data.LevelID == LevelItemWidget->GetLevelData().LevelID;
            LevelItemWidget->IsLevelSelected(IsSelected);
        }
    }
}

void UFMBMenuWidget::OnStartGame()
{
    const auto GameInstance = GetGameInstance();
    if (!GameInstance) return;

    UGameplayStatics::OpenLevel(this, GameInstance->GetStartLevel().LevelName);
}

void UFMBMenuWidget::OnQuitGame()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

UFMBGameInstance* UFMBMenuWidget::GetGameInstance() const
{
    if (!GetWorld()) return nullptr;
    return GetWorld()->GetGameInstance<UFMBGameInstance>();
}
