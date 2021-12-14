// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBPauseWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBPauseWidget : public UUserWidget
{
    GENERATED_BODY()
protected:
    UPROPERTY(meta =(BindWidget))
    UButton* ClearPauseButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnPauseClear();
};
