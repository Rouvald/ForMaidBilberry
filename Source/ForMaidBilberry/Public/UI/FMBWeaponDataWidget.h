// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBCoreTypes.h"
#include "FMBWeaponDataWidget.generated.h"

class UImage;
class USizeBox;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBWeaponDataWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    FOnWeaponSelectedSignature OnWeaponSelected;

    void SetWeaponUIData(const FWeaponUIData& Data);
    const FWeaponUIData& GetWeaponUIData() const { return WeaponUIData; }

    void WeaponIsSelected(bool IsSelected);

protected:
    UPROPERTY(meta = (BindWidget))
    USizeBox* WeaponUIDataSizeBox;

    UPROPERTY(meta = (BindWidget))
    UImage* WeaponUIImage;

    /*UPROPERTY(meta = (BindWidget))
    UImage* FrameImage;*/

private:
    FWeaponUIData WeaponUIData;

    float NormalSizeBox = 150.0f;
    float SelectedSizeBox = 300.0f;
};
