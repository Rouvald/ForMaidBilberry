// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
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
};
