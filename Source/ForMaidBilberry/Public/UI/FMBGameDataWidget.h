// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBGameDataWidget.generated.h"

class AFMBPlayerState;
class AFMBGameModeBase;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBGameDataWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    FText GetKillsNum() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    FText GetEnemiesNum() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    FText GetDayTime() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    ESlateVisibility IsPlayerAlone() const;

    /*UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetIsInfinityGame() const;*/

private:
    AFMBGameModeBase* GetGameModeBase() const;

    AFMBPlayerState* GetPlayerState() const;
};
