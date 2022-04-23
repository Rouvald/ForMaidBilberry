// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "FMBCoreTypes.h"
#include "FMBItemIconWidget.generated.h"

class UImage;
class UTextBlock;
class USizeBox;
class UFMBPlayerWeaponComponent;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBItemIconWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void SetItemImage(UTexture2D* ItemImage) const;
    void SetItemCount(const int32 NewItemCount) const;
    void SetItemIconSize(EItemType ItemType) const;

    void SetVisibleItemImage(const bool bIsVisible) const;

    void SetItemIsSelected(const bool bIsVisible) const;
    void SetItemCountVisible(const bool bIsVisible) const;

    FORCEINLINE const FItemData& GetItemData() const { return ItemData; }

protected:
    FItemData ItemData;

    UPROPERTY(meta = (BindWidget))
    USizeBox* ItemFrameSize{nullptr};

    UPROPERTY(meta = (BindWidget))
    USizeBox* ItemIconSize{nullptr};

    UPROPERTY(meta = (BindWidget))
    UImage* ItemImage{nullptr};

    UPROPERTY(meta = (BindWidget))
    UImage* SelectedItemFrame{nullptr};

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemCountTextBlock{nullptr};

    UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (ClampMin = "50.0", ClampMax = "150.0", ToolTip = "Nornal weapon icon size: 130."))
    float WeaponItemIconSize{130.0f};

    UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (ClampMin = "50.0", ClampMax = "150.0", ToolTip = "Normal pickUp icon size: 80"))
    float PickUpItemIconSize{80.0f};

    UPROPERTY()
    TMap<EItemType, float> ItemIconSizeMap;

    float FrameSize{5.0f};

    virtual void NativeOnInitialized() override;
};
