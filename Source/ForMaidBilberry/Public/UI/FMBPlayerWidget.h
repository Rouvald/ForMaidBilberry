// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "FMBCoreTypes.h"
#include "Blueprint/UserWidget.h"
#include "UI/FMBBaseWidget.h"
#include "FMBPlayerWidget.generated.h"

class UVerticalBox;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBPlayerWidget : public UFMBBaseWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(Category = "UI")
    float GetHealthPercent() const;

    UFUNCTION(Category = "UI")
    float GetStaminaPercent() const;

    /*UFUNCTION(Category = "UI")
    UTexture2D* GetCurrentWeaponUIImage() const;*/

    // UFUNCTION(BlueprintCallable, Category = "UI")
    // bool GetArmoryWeaponUIData(FItemData& WeaponUIData) const;

    UFUNCTION(Category = "UI")
    ESlateVisibility IsPlayerAlive() const;

    UFUNCTION(Category = "UI")
    ESlateVisibility IsPlayerSpectating() const;

    /*UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsFPPCamera() const;*/

    /*UFUNCTION(BlueprintCallable, Category = "UI")
    ESlateVisibility CrossHairVisibility() const;*/

    /*UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void OnTakeDamage();*/

protected:
    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* BloodyScreenAnim{nullptr};

    virtual void NativeOnInitialized() override;

private:
    void OnHealthChange(float Health, float HealthDelta);
    void OnNewPawn(APawn* NewPawn);
};
