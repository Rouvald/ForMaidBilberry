// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBCoreTypes.h"
#include "FMBWeaponItemBoxWidget.generated.h"

class UFMBWeaponComponent;
class UHorizontalBox;
class UFMBWeaponDataWidget;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBWeaponItemBoxWidget : public UUserWidget
{
    GENERATED_BODY()

    /*protected:
        UPROPERTY(meta = (BindWidget))
        UHorizontalBox* WeaponUIItemsBox;

        UPROPERTY(EditDefaultsOnly, Category = "UI")
        TSubclassOf<UUserWidget> WeaponUIDataWidgetClass;

        virtual void NativeOnInitialized() override;

    private:
        UPROPERTY()
        TArray<UFMBWeaponDataWidget*> WeaponUIDataWidgets;

        void InitWeaponItem();
        void OnWeaponSelected(const FWeaponUIData& Data);

        UFMBWeaponComponent* GetWeaponComponent() const;*/
};
