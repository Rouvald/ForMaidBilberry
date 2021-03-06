// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBHealthBarWidget.generated.h"

class UProgressBar;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBHealthBarWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void SetHealthPercent(float HealthPercent) const;

protected:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthProgressBar{nullptr};

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    float HealthBarVisibleThreshold{0.8f};
};
