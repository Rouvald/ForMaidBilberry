// For Maid Bilberry Game. All Rights Reserved

#include "UI/FMBPickUpItemBoxWidget.h"
#include "Components/HorizontalBox.h"
#include "FMBUtils.h"
#include "Components/FMBPlayerWeaponComponent.h"
#include "UI/FMBItemIconWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBPickUpItemBoxWidget, All, All)

void UFMBPickUpItemBoxWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    InitPickUpItem();
}

void UFMBPickUpItemBoxWidget::InitPickUpItem()
{
    PlayerWeaponComponent = GetWeaponComponent();
    if (!PlayerWeaponComponent) return;

    if (!PickUpIconBox) return;
    PickUpIconBox->ClearChildren();

    const auto ItemIconWidget = CreateWidget<UFMBItemIconWidget>(GetWorld(), PickUpIconWidgetClass);
    if (!ItemIconWidget) return;

    PlayerWeaponComponent->OnPickUpPickedUp.AddUObject(this, &UFMBPickUpItemBoxWidget::OnPickUpPickedUp);
    PlayerWeaponComponent->OnPickUpCountChange.AddUObject(this, &UFMBPickUpItemBoxWidget::OnPickUpCountChange);
    ItemIconWidget->SetVisibleItemImage(false);
    ItemIconWidget->SetSmallItemIconSize();

    PickUpIconBox->AddChild(ItemIconWidget);
    PickUpIconWidgets.Add(ItemIconWidget);

    // UE_LOG(LogFMBWeaponItemBoxWidget, Error, TEXT("%i"), WeaponIconBox->HasAnyChildren()) ;
}

void UFMBPickUpItemBoxWidget::OnPickUpPickedUp(const FItemData& Data)
{
    /* todo: in future rework for many PickUps. WeaponItemBox has same logic*/
    if (PickUpIconWidgets.Num() != 0 && PickUpIconWidgets[0])
    {
        PickUpIconWidgets[0]->ItemIsSelected(true);
        PickUpIconWidgets[0]->SetVisibleItemImage(true);
        PickUpIconWidgets[0]->SetItemImage(Data.ItemIcon);
    }
}

void UFMBPickUpItemBoxWidget::OnPickUpCountChange(int32 PickUpCount)
{
    if (PickUpIconWidgets[0] && PickUpCount > 1)
    {
        PickUpIconWidgets[0]->SetItemCount(PickUpCount);
    }
}

UFMBPlayerWeaponComponent* UFMBPickUpItemBoxWidget::GetWeaponComponent() const
{
    return FMBUtils::GetFMBPlayerComponent<UFMBPlayerWeaponComponent>(GetOwningPlayerPawn());
}
