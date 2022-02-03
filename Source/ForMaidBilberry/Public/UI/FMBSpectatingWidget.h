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
    UFUNCTION(BlueprintCallable, Category = "UI")
    FText GetRespawnTime() const;

private:
    AFMBGameModeBase* GetGameModeBase() const;
};
