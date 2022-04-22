// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBCoreTypes.h"
#include "FMBWeaponItemBoxWidget.generated.h"

class UFMBPlayerWeaponComponent;
class UHorizontalBox;
class UFMBItemIconWidget;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBWeaponItemBoxWidget : public UUserWidget
{
    GENERATED_BODY()
protected:
    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* WeaponIconBox;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> WeaponIconWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    UFMBPlayerWeaponComponent* PlayerWeaponComponent{nullptr};

    UPROPERTY()
    TArray<UFMBItemIconWidget*> WeaponIconWidgets;

    void InitWeaponItem();
    void OnWeaponPickedUp(int8 WeaponIndex, const FItemData& Data);
    void OnWeaponSelected(int8 WeaponIndex, const FItemData& Data, bool bIsVisible);
    void OnWeaponIconVisibility(int8 WeaponIndex, const FItemData& Data, bool bIsVisible);

    UFMBPlayerWeaponComponent* GetWeaponComponent() const;
};
