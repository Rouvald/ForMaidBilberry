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

    for (int8 Index = 0; Index < PlayerWeaponComponent->GetMaxWeapons(); ++Index)
    {
        const auto WeaponIconWidget = CreateWidget<UFMBItemIconWidget>(GetWorld(), WeaponIconWidgetClass);
        if (!WeaponIconWidget) continue;

        // WeaponUIDataWidget->SetWeaponUIData(Weapon->GetWeaponUIData());
        PlayerWeaponComponent->OnWeaponSelected.AddUObject(this, &UFMBWeaponItemBoxWidget::OnWeaponSelected);
        PlayerWeaponComponent->OnWeaponPickedUp.AddUObject(this, &UFMBWeaponItemBoxWidget::OnWeaponPickedUp);
        WeaponIconWidget->SetVisibleItemImage(false);

        WeaponIconBox->AddChild(WeaponIconWidget);
        WeaponIconWidgets.Add(WeaponIconWidget);
    }
    // UE_LOG(LogFMBWeaponItemBoxWidget, Error, TEXT("%i"), WeaponIconBox->HasAnyChildren()) ;
}

void UFMBWeaponItemBoxWidget::OnWeaponSelected(int8 WeaponIndex)
{
    if (WeaponIndex < WeaponIconWidgets.Num())
    {
        WeaponIconWidgets[WeaponIndex]->ItemIsSelected(true);
    }
}

void UFMBWeaponItemBoxWidget::OnWeaponPickedUp(int8 WeaponIndex, const FItemData& Data)
{
    if (WeaponIconWidgets[WeaponIndex])
    {
        WeaponIconWidgets[WeaponIndex]->SetVisibleItemImage(true);
        WeaponIconWidgets[WeaponIndex]->SetItemImage(Data.ItemIcon);
    }
}

UFMBPlayerWeaponComponent* UFMBWeaponItemBoxWidget::GetWeaponComponent() const
{
    return FMBUtils::GetFMBPlayerComponent<UFMBPlayerWeaponComponent>(GetOwningPlayerPawn());
}