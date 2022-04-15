// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "FMBItemIconWidget.generated.h"

class UImage;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBItemIconWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void SetWeaponUiImage(const UTexture2D* WeaponImage) const;

protected:
    UPROPERTY(meta = (BindWidget))
    UImage* WeaponUiImage{nullptr};

    virtual void NativeOnInitialized() override;
};
