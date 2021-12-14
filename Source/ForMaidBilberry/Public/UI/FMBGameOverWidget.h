// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBCoreTypes.h"
#include "FMBGameOverWidget.generated.h"

class UVerticalBox;
class UFMBPlayerStatRowWidget;
/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual bool Initialize() override;

protected:
    UPROPERTY(meta =(BindWidget))
    UVerticalBox* PlayerStatBox;

    UPROPERTY(EditDefaultsOnly, Category="UI")
    TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;

private:
    void OnMatchStateChange(EFMBMatchState State);
    void UpdatePlayerStat();
};
