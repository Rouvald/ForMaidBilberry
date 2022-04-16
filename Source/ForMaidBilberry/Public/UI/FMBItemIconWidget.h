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
    void SetBigItemIconSize() const;
    void SetSmallItemIconSize() const;

    void SetVisibleItemImage(const bool bIsVisible) const;

    void ItemIsSelected(const bool bIsSelected) const;

    FORCEINLINE const FItemData& GetItemData() const { return ItemData; }

protected:
    FItemData ItemData;

    UPROPERTY(meta = (BindWidget))
    USizeBox* ItemIconSize{nullptr};

    UPROPERTY(meta = (BindWidget))
    UImage* ItemImage{nullptr};

    UPROPERTY(meta = (BindWidget))
    UImage* SelectedItemFrame{nullptr};

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemCountTextBlock{nullptr};

    UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (ClampMin = "50.0", ClampMax = "150.0"))
    FVector2D BigItemIconSize{130.0f, 130.0f};

    UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (ClampMin = "50.0", ClampMax = "150.0"))
    FVector2D SmallItemIconSize{80.0f, 80.0f};

    virtual void NativeOnInitialized() override;
};
