// For Maid Bilberry Game. All Rights Recerved

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
    void SetHealthPercent(float HealthPercent);

protected:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthProgressBar;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    float HealthBarVisibleThreshold = 0.8f;
};
