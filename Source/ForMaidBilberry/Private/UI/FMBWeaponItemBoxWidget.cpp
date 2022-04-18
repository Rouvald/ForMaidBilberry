// For Maid Bilberry Game. All Rights Recerved

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

    InitWeaponItem();
}

void UFMBWeaponItemBoxWidget::InitWeaponItem()
{
    PlayerWeaponComponent = GetWeaponComponent();
    if (!PlayerWeaponComponent) return;

    if (!WeaponIconBox) return;
    WeaponIconBox->ClearChildren();

    WeaponIconWidgets.Init(nullptr, PlayerWeaponComponent->GetMaxWeapons());

    for (int8 Index = 0; Index < PlayerWeaponComponent->GetMaxWeapons(); ++Index)
    {
        const auto WeaponIconWidget = CreateWidget<UFMBItemIconWidget>(GetWorld(), WeaponIconWidgetClass);
        if (!WeaponIconWidget) continue;

        PlayerWeaponComponent->OnItemPickedUp.AddUObject(this, &UFMBWeaponItemBoxWidget::OnWeaponPickedUp);
        PlayerWeaponComponent->OnItemSelected.AddUObject(this, &UFMBWeaponItemBoxWidget::OnWeaponSelected);
        PlayerWeaponComponent->OnItemThrow.AddUObject(this, &UFMBWeaponItemBoxWidget::OnWeaponThrow);
        WeaponIconWidget->SetVisibleItemImage(false);

        WeaponIconBox->AddChild(WeaponIconWidget);
        WeaponIconWidgets[Index] = WeaponIconWidget;
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
    }
}

void UFMBWeaponItemBoxWidget::OnWeaponSelected(int8 WeaponIndex, const FItemData& Data)
{
    if (Data.ItemType != EItemType::EIT_Weapon) return;

    if (WeaponIconWidgets[WeaponIndex] && WeaponIconWidgets.Num() > WeaponIndex)
    {
        WeaponIconWidgets[WeaponIndex]->ItemIsSelected(true);
    }
}

void UFMBWeaponItemBoxWidget::OnWeaponThrow(int8 WeaponIndex, const FItemData& Data)
{
    if (Data.ItemType != EItemType::EIT_Weapon) return;

    if (WeaponIconWidgets[WeaponIndex] && WeaponIconWidgets.Num() > WeaponIndex)
    {
        WeaponIconWidgets[WeaponIndex]->SetVisibleItemImage(false);
    }
}

UFMBPlayerWeaponComponent* UFMBWeaponItemBoxWidget::GetWeaponComponent() const
{
    return FMBUtils::GetFMBPlayerComponent<UFMBPlayerWeaponComponent>(GetOwningPlayerPawn());
}