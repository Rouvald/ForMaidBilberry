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

    UFUNCTION(BlueprintCallable, Category = "UI")
    FText GetItemFastAttackDamage() const;
    void SetItemFastAttackDamage(float NewFastAttackDamage);

    UFUNCTION(BlueprintCallable, Category = "UI")
    FText GetItemStrongAttackDamage() const;
    void SetItemStrongAttackDamage(float NewStrongAttackDamage);

    UFUNCTION(BlueprintCallable, Category = "UI")
    FText GetItemHealthAmount() const;
    void SetItemHealthAmount(float NewHealthAmount);

    void SetItemImage(UTexture2D* ItemTexture2D) const;

protected:
    virtual void NativeOnInitialized() override;

private:
    UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
    UTextBlock* ItemName{nullptr};

    UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
    UImage* ItemImage{nullptr};

    float FastAttackDamage{0.0f};
    float StrongAttackDamage{0.0f};
    float HealthAmount{0.0f};
};
