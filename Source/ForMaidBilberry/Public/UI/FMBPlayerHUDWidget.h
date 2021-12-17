// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "FMBCoreTypes.h"
#include "Blueprint/UserWidget.h"
#include "FMBPlayerHUDWidget.generated.h"

class UFMBWeaponComponent;
/*class UHorizontalBox;
class UFMBGameInstance;
class UFMBWeaponDataWidget;*/
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

    UFUNCTION(BlueprintImplementableEvent, Category="UI")
    void OnTakeDamage();

protected:
    /*UPROPERTY(meta = (BindWidget))
    UHorizontalBox* WeaponUIItemsBox;

    UPROPERTY(EditDefaultsOnly, Category="UI")
    TSubclassOf<UUserWidget> WeaponUIDataWidgetClass;*/
    
    virtual void NativeOnInitialized() override;

private:
    /*UPROPERTY()
    TArray<UFMBWeaponDataWidget*> WeaponUIDataWidgets;*/
    
    void OnHealthChange(float Health, float HealthDelta);
    void OnNewPawn(APawn* NewPawn);

    /*void InitWeaponItem();
    void OnWeaponSelected(const FWeaponUIData& Data);*/
    UFMBWeaponComponent* GetWeaponComponent() const;
};
