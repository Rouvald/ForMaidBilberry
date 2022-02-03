// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "FMBCoreTypes.h"
#include "Blueprint/UserWidget.h"
#include "UI/FMBBaseWidget.h"
#include "FMBPlayerHUDWidget.generated.h"

/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBPlayerHUDWidget : public UFMBBaseWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetStaminaPercent() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponUIData(FWeaponUIData& WeaponUIData) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetArmoryWeaponUIData(FWeaponUIData& WeaponUIData) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    ESlateVisibility IsPlayerAlive() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    ESlateVisibility IsPlayerSpectating() const;

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
