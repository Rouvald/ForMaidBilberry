// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBItemInfoWidget.generated.h"

class UTextBlock;
class UImage;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBItemInfoWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void SetItemName(const FName& Name) const;

    UFUNCTION(BlueprintGetter, Category = "UI")
    FText GetItemProperty() const;
    void SetItemProperty(float NewProperty);

    UFUNCTION(BlueprintGetter, Category = "UI")
    FSlateColor GetItemPropertyColor() const;

    /*UFUNCTION(BlueprintCallable, Category = "UI")
    FText GetItemStrongAttackDamage() const;
    void SetItemStrongAttackDamage(float NewStrongAttackDamage);*/

    /*UFUNCTION(BlueprintCallable, Category = "UI")
    FText GetItemHealthAmount() const;
    void SetItemHealthAmount(float NewHealthAmount);*/

    void SetItemImage(UTexture2D* ItemTexture2D) const;

protected:
    virtual void NativeOnInitialized() override;

private:
    UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
    UTextBlock* ItemName{nullptr};

    UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
    UImage* ItemImage{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
    FLinearColor ItemInfoColorDefault{0.541667f, 0.257621f, 0.059451f, 1.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
    FLinearColor ItemInfoColorRed{1.0f, 0.055f, 0.12f, 1.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
    FLinearColor ItemInfoColorGreen{0.01f, 1.0f, 0.055f, 1.0f};

    float Property{0.0f};
};
