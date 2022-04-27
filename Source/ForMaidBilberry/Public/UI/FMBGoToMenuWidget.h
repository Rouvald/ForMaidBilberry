// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBGoToMenuWidget.generated.h"

class UButton;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBGoToMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* GoToMainMenuButton{nullptr};

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnGoToMenuButtonClicked();
};
