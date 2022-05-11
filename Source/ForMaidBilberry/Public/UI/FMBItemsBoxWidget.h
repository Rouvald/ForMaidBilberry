// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBCoreTypes.h"
#include "FMBItemsBoxWidget.generated.h"

class UHorizontalBox;
class UFMBPlayerWeaponComponent;
class UFMBItemIconWidget;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBItemsBoxWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    /* Initialize item icons widgets */
    void FillItemIcons(const EItemType NewItemType);
    void SetWidgetItemType(const EItemType NewWidgetItemType) { WidgetItemType = NewWidgetItemType; }
    void SetItemsIconSize(const EItemType ItemType);

protected:
    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* ItemIconBox{nullptr};

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> ItemIconWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    UFMBPlayerWeaponComponent* PlayerWeaponComponent{nullptr};

    UPROPERTY()
    TArray<UFMBItemIconWidget*> ItemIconWidgets;

    UPROPERTY()
    EItemType WidgetItemType{EItemType::EIT_Max};

    int8 WidgetItemIcons{0};

    void OnItemPickedUp(int8 ItemIndex, const FItemData& Data);
    void OnItemSelected(int8 ItemIndex, const EItemType ItemType, bool bIsVisible);
    void OnItemCountChange(int8 ItemIndex, const FItemData& Data);
    void OnItemCountVisibility(int8 ItemIndex, const EItemType ItemType, bool bIsVisible);
    void OnItemIconVisibility(int8 ItemIndex, const EItemType ItemType, bool bIsVisible);

    void OnNewPawn(APawn* Pawn);
};
