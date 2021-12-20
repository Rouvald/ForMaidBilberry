// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBBaseWidget.generated.h"

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
    UWidgetAnimation* ShowAnim;
};
