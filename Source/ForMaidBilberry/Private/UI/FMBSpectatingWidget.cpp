// For Maid Bilberry Game. All Rights Recerved

#include "UI/FMBSpectatingWidget.h"
#include "FMBGameModeBase.h"
#include "FMBRespawnComponent.h"
#include "FMBUtils.h"

bool UFMBSpectatingWidget::GetRespawnTime(int32& RespawnCountDownTime) const
{
    const auto RespawnComponent = FMBUtils::GetFMBPlayerComponent<UFMBRespawnComponent>(GetOwningPlayer());
    if (!RespawnComponent || !RespawnComponent->IsRespawnStarted()) return false;

    RespawnCountDownTime = RespawnComponent->GetRespawnCountDown();
    return true;
}

bool UFMBSpectatingWidget::GetGameplayTimeRemaining(int32& GameplayTimeRemaining) const
{
    const auto GameMode = GetGameModeBase();
    if (!GameMode) return false;

    GameplayTimeRemaining = GameMode->GetGameTimer();
    return true;
}

AFMBGameModeBase* UFMBSpectatingWidget::GetGameModeBase() const
{
    return GetWorld() ? Cast<AFMBGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}