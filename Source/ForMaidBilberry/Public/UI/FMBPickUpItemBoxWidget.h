// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBCoreTypes.h"
#include "FMBPickUpItemBoxWidget.generated.h"

class UHorizontalBox;
class UFMBPlayerWeaponComponent;
class UFMBItemIconWidget;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBPickUpItemBoxWidget : public UUserWidget
{
    GENERATED_BODY()
protected:
    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* PickUpIconBox;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> PickUpIconWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    UFMBPlayerWeaponComponent* PlayerWeaponComponent{nullptr};

    UPROPERTY()
    TArray<UFMBItemIconWidget*> PickUpIconWidgets;

    void InitPickUpItem();
    void OnPickUpPickedUp(const FItemData& Data);
    void OnPickUpCountChange(int32 PickUpCount);

    UFMBPlayerWeaponComponent* GetWeaponComponent() const;
};
