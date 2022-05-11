// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "FMBCoreTypes.h"
#include "Blueprint/UserWidget.h"
#include "UI/FMBBaseWidget.h"
#include "FMBPlayerWidget.generated.h"

class UVerticalBox;
class UFMBItemsBoxWidget;
class USpacer;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBPlayerWidget : public UFMBBaseWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetStaminaPercent() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    ESlateVisibility IsPlayerAlive() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    ESlateVisibility IsPlayerSpectating() const;
    /*UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void OnTakeDamage();*/

protected:
    virtual void NativeOnInitialized() override;

private:
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* ItemsVerticalBox{nullptr};

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* BloodyScreenAnim{nullptr};

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> ItemBoxWidgetClass;

    void InitItemVerticalBoxes() const;
    UFMBItemsBoxWidget* CreateItemBoxWidget(const EItemType ItemBoxType) const;
    USpacer* CreateSpacer(const FVector2D& SpacerSize) const;
    void OnHealthChange(float Health, float HealthDelta);
    void OnNewPawn(APawn* NewPawn);
};
