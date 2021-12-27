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
    int32 GetKillsNum() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetEnemiesNum(int32& AllEnemiesNum) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetGameplayTimeRemaining() const;

    /*UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetIsInfinityGame() const;*/

private:
    AFMBGameModeBase* GetGameModeBase() const;

    AFMBPlayerState* GetPlayerState() const;
};
