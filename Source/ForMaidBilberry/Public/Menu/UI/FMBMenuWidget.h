// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBCoreTypes.h"
#include "FMBMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class UFMBGameInstance;
class UFMBLevelItemWidget;
/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta=(BindWidget))
    UButton* StartGameButton;

    UPROPERTY(meta=(BindWidget))
    UButton* QuitGameButton;

    UPROPERTY(meta=(BindWidget))
    UHorizontalBox* LevelItemsBox;

    UPROPERTY(EditDefaultsOnly, Category="UI")
    TSubclassOf<UUserWidget> LevelItemWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    TArray<UFMBLevelItemWidget*> LevelItemWidgets;
    
    UFUNCTION()
    void OnStartGame();

    UFUNCTION()
    void OnQuitGame();

    void InitLevelItem();
    void OnLevelSelected(const FLevelData& Data);
    UFMBGameInstance* GetGameInstance() const;
};
