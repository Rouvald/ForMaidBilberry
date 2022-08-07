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
    /* Initialize items box widget */
    void InitItemsBoxWidget(EItemType ItemType);

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

    int8 WidgetItemIconsAmount{0};

    /* Functions for initialize items box widget */
    void FillItemIcons(EItemType NewItemType);
    void SetItemsIconSize(EItemType ItemType);

    void SetWidgetItemType(EItemType NewWidgetItemType) { WidgetItemType = NewWidgetItemType; }

    /* Callback func */
    void OnItemPickedUp(int8 ItemIndex, const FItemData& Data);
    void OnItemSelected(int8 ItemIndex, const EItemType ItemType, bool bIsVisible);
    void OnItemCountChange(int8 ItemIndex, const FItemData& Data);
    void OnItemCountVisibility(int8 ItemIndex, const EItemType ItemType, bool bIsVisible);
    void OnItemIconVisibility(int8 ItemIndex, const EItemType ItemType, bool bIsVisible);

    void OnNewPawn(APawn* Pawn);
};
