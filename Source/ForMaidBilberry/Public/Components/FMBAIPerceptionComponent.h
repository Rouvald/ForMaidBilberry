// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "FMBAIPerceptionComponent.generated.h"

/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBAIPerceptionComponent : public UAIPerceptionComponent
{
    GENERATED_BODY()

public:
    AActor* GetEnemyPlayer() const;
};
