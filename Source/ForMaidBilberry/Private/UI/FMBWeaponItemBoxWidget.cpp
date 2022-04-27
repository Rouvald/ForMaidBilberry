// For Maid Bilberry Game. All Rights Reserved

#include "UI/FMBWeaponItemBoxWidget.h"
#include "FMBUtils.h"
#include "Weapon/FMBBaseWeapon.h"
#include "Components/FMBPlayerWeaponComponent.h"
#include "Components/HorizontalBox.h"
#include "UI/FMBItemIconWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBWeaponItemBoxWidget, All, All)

void UFMBWeaponItemBoxWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UFMBWeaponItemBoxWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
    InitWeaponItem();
}

void UFMBWeaponItemBoxWidget::OnNewPawn(APawn* Pawn)
{
    PlayerWeaponComponent = FMBUtils::GetFMBPlayerComponent<UFMBPlayerWeaponComponent>(Pawn);
    if (PlayerWeaponComponent)
    {
        PlayerWeaponComponent->OnItemPickedUp.AddUObject(this, &UFMBWeaponItemBoxWidget::OnWeaponPickedUp);
        PlayerWeaponComponent->OnItemSelected.AddUObject(this, &UFMBWeaponItemBoxWidget::OnWeaponSelected);
        PlayerWeaponComponent->OnItemIconVisibility.AddUObject(this, &UFMBWeaponItemBoxWidget::OnWeaponIconVisibility);
        /*PlayerWeaponComponent->OnItemCountChange.AddUObject(this, &UFMBWeaponItemBoxWidget::OnWeaponCountChange);
        PlayerWeaponComponent->OnItemCountVisible.AddUObject(this, &UFMBWeaponItemBoxWidget::OnWeaponCountVisible);*/
    }
}

void UFMBWeaponItemBoxWidget::InitWeaponItem()
{
    if (!PlayerWeaponComponent) return;

    if (!WeaponIconBox) return;
    WeaponIconBox->ClearChildren();

    WeaponIconWidgets.Init(nullptr, PlayerWeaponComponent->GetMaxWeapons());

    for (int8 Index = 0; Index < PlayerWeaponComponent->GetMaxWeapons(); ++Index)
    {
        const auto WeaponIconWidget = CreateWidget<UFMBItemIconWidget>(GetWorld(), WeaponIconWidgetClass);
        if (!WeaponIconWidget) continue;

        WeaponIconWidget->SetVisibleItemImage(false);
        WeaponIconWidget->SetItemIconSize(EItemType::EIT_Weapon);

        WeaponIconBox->AddChild(WeaponIconWidget);
        WeaponIconWidgets[Index] = WeaponIconWidget;
    }
    if (WeaponIconWidgets[0])
    {
        WeaponIconWidgets[0]->SetItemIsSelected(true);
    }
    // UE_LOG(LogFMBWeaponItemBoxWidget, Error, TEXT("%i"), WeaponIconBox->HasAnyChildren()) ;
}

void UFMBWeaponItemBoxWidget::OnWeaponPickedUp(int8 WeaponIndex, const FItemData& Data)
{
    if (Data.ItemType != EItemType::EIT_Weapon) return;

    if (WeaponIconWidgets[WeaponIndex] && WeaponIconWidgets.Num() > WeaponIndex)
    {
        WeaponIconWidgets[WeaponIndex]->SetVisibleItemImage(true);
        WeaponIconWidgets[WeaponIndex]->SetItemImage(Data.ItemIcon);
        /*UE_LOG(LogFMBWeaponItemBoxWidget, Error, TEXT("On Weapon PickedUp"));*/
    }
}

void UFMBWeaponItemBoxWidget::OnWeaponSelected(int8 WeaponIndex, const EItemType ItemType, bool bIsVisible)
{
    if (ItemType != EItemType::EIT_Weapon) return;
    // UE_LOG(LogFMBWeaponItemBoxWidget, Error, TEXT("UFMBWeaponItemBoxWidget::OnWeaponSelected"));
    if (WeaponIconWidgets[WeaponIndex] && WeaponIconWidgets.Num() > WeaponIndex)
    {
        WeaponIconWidgets[WeaponIndex]->SetItemIsSelected(bIsVisible);
    }
}

void UFMBWeaponItemBoxWidget::OnWeaponIconVisibility(int8 WeaponIndex, const EItemType ItemType, bool bIsVisible)
{
    if (ItemType != EItemType::EIT_Weapon) return;

    if (WeaponIconWidgets[WeaponIndex] && WeaponIconWidgets.Num() > WeaponIndex)
    {
        WeaponIconWidgets[WeaponIndex]->SetVisibleItemImage(bIsVisible);
    }
}

/*void UFMBWeaponItemBoxWidget::OnWeaponCountChange(int8 WeaponIndex, const FItemData& Data)
{
    if (Data.ItemType != EItemType::EIT_PickUp) return;

    if (WeaponIconWidgets[WeaponIndex] && WeaponIconWidgets.Num() > WeaponIndex)
    {
        WeaponIconWidgets[WeaponIndex]->SetItemCount(Data.ItemCount);
    }
}

void UFMBWeaponItemBoxWidget::OnWeaponCountVisible(int8 WeaponIndex, const FItemData& Data, bool bIsVisible)
{
    if (Data.ItemType != EItemType::EIT_PickUp) return;

    if (WeaponIconWidgets[WeaponIndex] && WeaponIconWidgets.Num() > WeaponIndex)
    {
        WeaponIconWidgets[WeaponIndex]->SetItemCountVisible(bIsVisible);
    }
}*/

/*UFMBPlayerWeaponComponent* UFMBWeaponItemBoxWidget::GetWeaponComponent() const
{
    return FMBUtils::GetFMBPlayerComponent<UFMBPlayerWeaponComponent>(GetOwningPlayerPawn());
}*/
