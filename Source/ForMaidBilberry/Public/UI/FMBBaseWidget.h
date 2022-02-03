// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBBaseWidget.generated.h"

class USoundCue;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBBaseWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void Show();

protected:
    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* ShowAnim{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* OpenWidgetSound{nullptr};
};
