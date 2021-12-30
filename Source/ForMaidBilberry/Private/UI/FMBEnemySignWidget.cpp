// For Maid Bilberry Game. All Rights Recerved

#include "UI/FMBEnemySignWidget.h"

#include "Components/Image.h"

void UFMBEnemySignWidget::SetEnemySignVisibility(bool IsDead) const
{
    if (EnemySignImage)
    {
        EnemySignImage->SetVisibility(IsDead ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
    }
}
