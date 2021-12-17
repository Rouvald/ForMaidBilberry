// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBMenuWidget.generated.h"

class UButton;
class UFMBSelectLevelWidget;
/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBMenuWidget : public UUserWidget
{
    GENERATED_BODY()
protected:
    UPROPERTY(meta=(BindWidget))
    UButton* SelectLevelButton;

    UPROPERTY(meta=(BindWidget))
    UButton* QuitGameButton;

    /*UPROPERTY(meta=(BindWidget))
    UButton* SelectCharacterButton;*/

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnSelectLevelGame();

    UFUNCTION()
    void OnQuitGame();

    /*UFUNCTION()
    void OnSelectCharacterClicked();*/
};