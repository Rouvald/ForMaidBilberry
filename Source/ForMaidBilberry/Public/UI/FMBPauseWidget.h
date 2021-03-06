// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/FMBBaseWidget.h"
#include "FMBPauseWidget.generated.h"

class UButton;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBPauseWidget : public UFMBBaseWidget
{
    GENERATED_BODY()
protected:
    UPROPERTY(meta = (BindWidget))
    UButton* ClearPauseButton{nullptr};

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnPauseClear();
};
