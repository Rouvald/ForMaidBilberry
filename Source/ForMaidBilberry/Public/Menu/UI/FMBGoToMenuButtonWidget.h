// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBGoToMenuButtonWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBGoToMenuButtonWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta=(BindWidget))
    UButton* BackToMenuButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnBackToMenu();
};
