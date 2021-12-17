// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBCoreTypes.h"
#include "FMBSelectLevelWidget.generated.h"

class UButton;
class UVerticalBox;
class UFMBGameInstance;
class UFMBLevelItemWidget;
/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBSelectLevelWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta=(BindWidget))
    UVerticalBox* LevelItemsBox;

    UPROPERTY(meta=(BindWidget))
    UButton* StartGameButton;
    
    UPROPERTY(EditDefaultsOnly, Category="UI")
    TSubclassOf<UUserWidget> LevelItemWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    TArray<UFMBLevelItemWidget*> LevelItemWidgets;

    UFUNCTION()
    void OnStartGame();

    void InitLevelItem();
    void OnLevelSelected(const FLevelData& Data);
    UFMBGameInstance* GetFMBGameInstance() const;
};
