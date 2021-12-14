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

public:
    virtual bool Initialize() override;

protected:
    UPROPERTY(meta =(BindWidget))
    UButton* ClearPauseButton;

private:
    UFUNCTION()
    void OnPauseClear();
};
