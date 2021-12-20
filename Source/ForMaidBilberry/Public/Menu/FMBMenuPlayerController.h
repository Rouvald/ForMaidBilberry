// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FMBMenuPlayerController.generated.h"

/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API AFMBMenuPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;
};
