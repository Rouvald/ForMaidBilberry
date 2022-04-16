// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "FMBCoreTypes.h"
#include "FMBItemIconWidget.generated.h"

class UImage;
class UTextBlock;
class UFMBPlayerWeaponComponent;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBItemIconWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void SetItemImage(UTexture2D* WeaponImage) const;
    void SetItemCount(const int32 NewItemCount) const;

    void SetVisibleItemImage(const bool bIsVisible) const;

    void ItemIsSelected(const bool bIsSelected) const;

    FORCEINLINE const FItemData& GetItemData() const { return ItemData; }

protected:
    FItemData ItemData;

    UPROPERTY(meta = (BindWidget))
    UImage* ItemImage{nullptr};

    UPROPERTY(meta = (BindWidget))
    UImage* SelectedItemFrame{nullptr};

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemCount{nullptr};

    virtual void NativeOnInitialized() override;
};
