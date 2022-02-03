// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBKeyboardInfoWidget.generated.h"

// class UTextBlock;
class UOverlay;
class AFMBPlayerController;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBKeyboardInfoWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UOverlay* KeyboardInfoOverlay{nullptr};

    UPROPERTY(meta = (BindWidget))
    UOverlay* KeyboardInfoVisibilityOverlay{nullptr};

    virtual void NativeOnInitialized() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    UFUNCTION(BlueprintCallable)
    bool GetIsKeyboardInfoVisible() const;

private:
    void ChangeKeyboardInfoVisibility() const;

    AFMBPlayerController* GetPlayerController() const;
};
