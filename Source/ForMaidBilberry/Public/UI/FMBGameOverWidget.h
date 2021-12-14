// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBCoreTypes.h"
#include "FMBGameOverWidget.generated.h"

class UVerticalBox;
class UButton;
class UFMBPlayerStatRowWidget;
/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
    UPROPERTY(meta =(BindWidget))
    UVerticalBox* PlayerStatBox;

    UPROPERTY(meta =(BindWidget))
    UButton* ResetLevelButton;

    UPROPERTY(EditDefaultsOnly, Category="UI")
    TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    void OnMatchStateChange(EFMBMatchState State);
    void UpdatePlayerStat();

    UFUNCTION()
    void OnResetLevel();
};
