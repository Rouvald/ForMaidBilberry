// For Maid Bilberry Game. All Rights Recerved

#include "UI/FMBHealthBarWidget.h"
#include "Components/ProgressBar.h"

void UFMBHealthBarWidget::SetHealthPercent(float HealthPercent) const
{
    if (HealthProgressBar)
    {
        const auto HealthBarVisibility = (HealthPercent > HealthBarVisibleThreshold || FMath::IsNearlyZero(HealthPercent))
                                             ? ESlateVisibility::Hidden
                                             : ESlateVisibility::Visible;

        HealthProgressBar->SetVisibility(HealthBarVisibility);

        HealthProgressBar->SetPercent(HealthPercent);
    }
}
