// For Maid Bilberry Game. All Rights Recerved

#include "UI/FMBSpectatingWidget.h"
#include "FMBGameModeBase.h"
#include "FMBRespawnComponent.h"
#include "FMBUtils.h"

FText UFMBSpectatingWidget::GetRespawnTime() const
{
    const auto RespawnComponent = FMBUtils::GetFMBPlayerComponent<UFMBRespawnComponent>(GetOwningPlayer());
    if (!RespawnComponent || !RespawnComponent->IsRespawnStarted()) return FText{FText::FromString(TEXT("Error"))};

    const int32 RespawnCountDownTime = RespawnComponent->GetRespawnCountDown();

    const FString RespawnTime{TEXT("Respawn in ") + FString::FromInt(RespawnCountDownTime)};
    return FText::FromString(RespawnTime);
}

AFMBGameModeBase* UFMBSpectatingWidget::GetGameModeBase() const
{
    return GetWorld() ? Cast<AFMBGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}
