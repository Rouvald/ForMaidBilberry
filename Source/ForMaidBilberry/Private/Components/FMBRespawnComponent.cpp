// For Maid Bilberry Game. All Rights Reserved

#include "Components/FMBRespawnComponent.h"
#include "FMBGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBRespawnComponent, All, All)

UFMBRespawnComponent::UFMBRespawnComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}
void UFMBRespawnComponent::Respawn(int32 RespawnTime)
{
    if (!GetWorld()) return;

    RespawnCountDown = RespawnTime;
    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &UFMBRespawnComponent::RespawnUpdateTimer, 1.0f, true);
}

void UFMBRespawnComponent::RespawnUpdateTimer()
{
    if (--RespawnCountDown == 0)
    {
        if (!GetWorld()) return;

        GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

        const auto GameMode = Cast<AFMBGameModeBase>(GetWorld()->GetAuthGameMode());
        if (!GameMode) return;

        GameMode->RespawnRequest(Cast<AController>(GetOwner()));
    }
}

bool UFMBRespawnComponent::IsRespawnStarted() const
{
    return GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}