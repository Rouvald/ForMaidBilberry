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

    PlayerWeaponComponent->OnItemPickedUp.AddUObject(this, &UFMBPickUpItemBoxWidget::OnPickUpPickedUp);
    PlayerWeaponComponent->OnItemSelected.AddUObject(this, &UFMBPickUpItemBoxWidget::OnPickUpSelected);
    PlayerWeaponComponent->OnItemCountChange.AddUObject(this, &UFMBPickUpItemBoxWidget::OnPickUpCountChange);
    PlayerWeaponComponent->OnItemThrow.AddUObject(this, &UFMBPickUpItemBoxWidget::OnPickUpThrow);
    ItemIconWidget->SetVisibleItemImage(false);
    ItemIconWidget->SetSmallItemIconSize();

    PickUpIconBox->AddChild(ItemIconWidget);
    PickUpIconWidgets.Add(ItemIconWidget);

    // UE_LOG(LogFMBWeaponItemBoxWidget, Error, TEXT("%i"), WeaponIconBox->HasAnyChildren()) ;
}

void UFMBPickUpItemBoxWidget::OnPickUpPickedUp(int8 PickUpIndex, const FItemData& Data)
{
    if (Data.ItemType != EItemType::EIT_PickUp) return;

    if (PickUpIconWidgets[PickUpIndex] && PickUpIconWidgets.Num() > PickUpIndex)
    {
        PickUpIconWidgets[PickUpIndex]->SetVisibleItemImage(true);
        PickUpIconWidgets[PickUpIndex]->SetItemImage(Data.ItemIcon);
    }
}

void UFMBPickUpItemBoxWidget::OnPickUpSelected(int8 PickUpIndex, const FItemData& Data)
{
    if (Data.ItemType != EItemType::EIT_PickUp) return;

    if (PickUpIconWidgets[PickUpIndex] && PickUpIconWidgets.Num() > PickUpIndex)
    {
        PickUpIconWidgets[PickUpIndex]->ItemIsSelected(true);
    }
}

void UFMBPickUpItemBoxWidget::OnPickUpCountChange(int8 PickUpIndex, const FItemData& Data)
{
    if (Data.ItemType != EItemType::EIT_PickUp) return;

    if (PickUpIconWidgets[PickUpIndex] && PickUpIconWidgets.Num() > PickUpIndex)
    {
        PickUpIconWidgets[PickUpIndex]->SetItemCount(Data.ItemCount);
    }
}

void UFMBPickUpItemBoxWidget::OnPickUpThrow(int8 PickUpIndex, const FItemData& Data)
{
    if (Data.ItemType != EItemType::EIT_PickUp) return;

    if (PickUpIconWidgets[PickUpIndex] && PickUpIconWidgets.Num() > PickUpIndex)
    {
        // PickUpIconWidgets[PickUpIndex]->ItemIsSelected(true);
        PickUpIconWidgets[PickUpIndex]->SetVisibleItemImage(false);
    }
}

UFMBPlayerWeaponComponent* UFMBPickUpItemBoxWidget::GetWeaponComponent() const
{
    return FMBUtils::GetFMBPlayerComponent<UFMBPlayerWeaponComponent>(GetOwningPlayerPawn());
}
