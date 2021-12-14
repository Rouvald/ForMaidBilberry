// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBPlayerStatRowWidget.generated.h"

class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBPlayerStatRowWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetPlayerName(const FText& Text);
    void SetKillsNum(const FText& Text);
    void SetDeathsNum(const FText& Text);
    void SetBackGroundImageVisibility(bool IsVisible);

protected:
    UPROPERTY(meta=(BindWidget))
    UTextBlock* PlayerNameText;

    UPROPERTY(meta=(BindWidget))
    UTextBlock* KillsNumText;

    UPROPERTY(meta=(BindWidget))
    UTextBlock* DeathsNumText;

    UPROPERTY(meta=(BindWidget))
    UImage* BackgroundImage;
};
