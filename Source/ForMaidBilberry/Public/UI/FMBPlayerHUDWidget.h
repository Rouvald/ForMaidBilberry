// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "FMBCoreTypes.h"
#include "Blueprint/UserWidget.h"
#include "FMBPlayerHUDWidget.generated.h"
/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBPlayerHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="UI")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category="UI")
    float GetStaminaPercent() const;

    UFUNCTION(BlueprintCallable, Category="UI")
    bool GetCurrentWeaponUIData(FWeaponUIData& WeaponUIData) const;

    UFUNCTION(BlueprintCallable, Category="UI")
    bool GetArmoryWeaponUIData(FWeaponUIData& WeaponUIData) const;

    UFUNCTION(BlueprintCallable, Category="UI")
    bool IsPlayerAlive() const;

    UFUNCTION(BlueprintCallable, Category="UI")
    bool IsPlayerSpectating() const;
};
