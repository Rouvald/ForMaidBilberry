// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBWeaponDataWidget.generated.h"
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBWeaponDataWidget : public UUserWidget
{
    GENERATED_BODY()
    /*public:
        FOnWeaponSelectedSignature OnWeaponSelected;

        void SetWeaponUIData(const FWeaponUIData& Data);
        const FWeaponUIData& GetWeaponUIData() const { return WeaponUIData; }

        void WeaponIsSelected(bool IsSelected);

    protected:
        /*UPROPERTY(meta = (BindWidget))
        UImage* WeaponImage;

        UPROPERTY(meta = (BindWidget))
        UImage* ShadowImage;#1#

        virtual void NativeOnInitialized() override;

    private:
        FWeaponUIData WeaponUIData;

        float NormalShadowOpacity = 0.5f;
        float SelectedShadowOpacity = 0.0f;*/
};
