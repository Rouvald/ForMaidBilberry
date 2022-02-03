// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FMBRespawnComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FORMAIDBILBERRY_API UFMBRespawnComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFMBRespawnComponent();

    void Respawn(int32 RespawnTime);

    FORCEINLINE int32 GetRespawnCountDown() const { return RespawnCountDown; }

    bool IsRespawnStarted() const;

private:
    FTimerHandle RespawnTimerHandle;
    int32 RespawnCountDown{0};

    void RespawnUpdateTimer();
};
