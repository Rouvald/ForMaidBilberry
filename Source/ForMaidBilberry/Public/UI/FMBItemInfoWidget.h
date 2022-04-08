// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBItemInfoWidget.generated.h"

class UTextBlock;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBItemInfoWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void SetItemName(const FName& Name) const;
    void SetItemDamage(float Damage) const;

protected:
    virtual void NativeOnInitialized() override;

private:
    UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
    UTextBlock* ItemName{nullptr};

    UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
    UTextBlock* ItemDamage{nullptr};
};
