// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FMBCoreTypes.h"
#include "FMBMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API AFMBMenuHUD : public AHUD
{
	GENERATED_BODY()

public:    
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category="UI")
    TSubclassOf<UUserWidget> MenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category="UI")
    TSubclassOf<UUserWidget> SelectLevelWidgetClass;

    /*UPROPERTY(EditDefaultsOnly, Category="UI")
    TSubclassOf<UUserWidget> SelectCharacterWidgetClass;*/

private:
    UPROPERTY()
    TMap<EFMBMenuState, UUserWidget*> MenuWidgets;

    UPROPERTY()
    UUserWidget* CurrentWidget = nullptr;
    
    void OnMenuStateChanged(EFMBMenuState State);

    //void SetCurrentWidgetVisibility(bool IsVisibility) const;
};
