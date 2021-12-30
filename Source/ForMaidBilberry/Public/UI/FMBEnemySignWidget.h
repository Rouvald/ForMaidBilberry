// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FMBEnemySignWidget.generated.h"

class UImage;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBEnemySignWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void SetEnemySignVisibility(bool IsDead) const;

protected:
    UPROPERTY(meta = (BindWidget))
    UImage* EnemySignImage;
};
