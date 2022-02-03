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
    void SetPlayerName(const FText& Text) const;
    void SetKillsNum(const FText& Text) const;
    void SetDeathsNum(const FText& Text) const;
    void SetBackGroundImageVisibility(bool IsVisible) const;

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* PlayerNameText{nullptr};

    UPROPERTY(meta = (BindWidget))
    UTextBlock* KillsNumText{nullptr};

    UPROPERTY(meta = (BindWidget))
    UTextBlock* DeathsNumText{nullptr};

    UPROPERTY(meta = (BindWidget))
    UImage* BackgroundImage{nullptr};
};
