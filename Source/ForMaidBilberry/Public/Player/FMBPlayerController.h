// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FMBPlayerController.generated.h"


class UFMBRespawnComponent;
/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API AFMBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    AFMBPlayerController();

protected:
    UPROPERTY(EditDefaultsOnly, Category="Component")
    UFMBRespawnComponent* RespawnComponent;
};
