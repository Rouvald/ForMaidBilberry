// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBSpectatingWidget.generated.h"

class AFMBGameModeBase;
/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBSpectatingWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category="UI")
    bool GetRespawnTime(int32& RespawnCountDownTime) const;

    UFUNCTION(BlueprintCallable, Category="UI")
    bool GetGameplayTimeRemaining(int32& GameplayTimeRemaining) const;

private:
    AFMBGameModeBase* GetGameModeBase() const;
};
