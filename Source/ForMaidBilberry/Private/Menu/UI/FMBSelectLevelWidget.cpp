// For Maid Bilberry Game. All Rights Recerved

#include "Menu/UI/FMBSelectLevelWidget.h"
#include "FMBGameInstance.h"
#include "FMBLevelItemWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void UFMBSelectLevelWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &UFMBSelectLevelWidget::OnStartGame);
    }
    InitLevelItem();
}

void UFMBSelectLevelWidget::InitLevelItem()
{
    const auto GameInstance = GetFMBGameInstance();
    if (!GameInstance) return;

    checkf(GameInstance->GetLevelsData().Num() != 0, TEXT("Levels data is EMPTY"));

    if (!LevelItemsBox) return;
    LevelItemsBox->ClearChildren();

    int32 LevelID = 0;
    for (auto LevelData : GameInstance->GetLevelsData())
    {
        const auto LevelItemWidget = CreateWidget<UFMBLevelItemWidget>(this, LevelItemWidgetClass);
        if (!LevelItemWidget) continue;

        LevelData.LevelID = LevelID;
        LevelItemWidget->SetLevelData(LevelData);
        LevelItemWidget->OnLevelSelected.AddUObject(this, &UFMBSelectLevelWidget::OnLevelSelected);

        LevelItemsBox->AddChild(LevelItemWidget);
        LevelItemWidgets.Add(LevelItemWidget);

        ++LevelID;
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

void UFMBSelectLevelWidget::OnLevelSelected(const FLevelData& Data)
{
    const auto GameInstance = GetFMBGameInstance();
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

void UFMBSelectLevelWidget::OnStartGame()
{
    PlayAnimation(LoadingLevelAnim);
    UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
}

void UFMBSelectLevelWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    if (Animation != LoadingLevelAnim) return;

    const auto GameInstance = GetFMBGameInstance();
    if (!GameInstance) return;

    UGameplayStatics::OpenLevel(this, GameInstance->GetStartLevel().LevelName);
}

UFMBGameInstance* UFMBSelectLevelWidget::GetFMBGameInstance() const
{
    if (!GetWorld()) return nullptr;
    return GetWorld()->GetGameInstance<UFMBGameInstance>();
}
