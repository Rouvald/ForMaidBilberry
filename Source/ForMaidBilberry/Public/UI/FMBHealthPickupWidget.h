// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBHealthPickupWidget.generated.h"

class UWidgetAnimation;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBHealthPickupWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void StartPlayingAnim();

protected:
    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* BlinkingAnim{nullptr};
};
