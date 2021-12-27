// For Maid Bilberry Game. All Rights Recerved

#include "UI/FMBWeaponItemBoxWidget.h"
#include "FMBUtils.h"
#include "Weapon/FMBBaseWeapon.h"
#include "Components/FMBWeaponComponent.h"
#include "Components/HorizontalBox.h"
#include "UI/FMBWeaponDataWidget.h"

/*DEFINE_LOG_CATEGORY_STATIC(LogFMBWeaponItemBoxWidget, All, All)

void UFMBWeaponItemBoxWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    InitWeaponItem();
}

void UFMBWeaponItemBoxWidget::InitWeaponItem()
{
    const auto WeaponComponent = GetWeaponComponent();
    if (!WeaponComponent) return;

    if (!WeaponUIItemsBox) return;
    WeaponUIItemsBox->ClearChildren();

    for (const auto WeaponClass : WeaponComponent->GetWeaponClasses())
    {
        const auto Weapon = Cast<AFMBBaseWeapon>(WeaponClass->GetDefaultObject());
        const auto WeaponUIDataWidget = CreateWidget<UFMBWeaponDataWidget>(GetWorld(), WeaponUIDataWidgetClass);
        if (!WeaponUIDataWidget || !Weapon) continue;

        //WeaponUIDataWidget->SetWeaponUIData(Weapon->GetWeaponUIData());
        //WeaponUIDataWidget->OnWeaponSelected.AddUObject(this, &UFMBWeaponItemBoxWidget::OnWeaponSelected);

        WeaponUIItemsBox->AddChild(WeaponUIDataWidget);
        //WeaponUIDataWidgets.Add(WeaponUIDataWidget);
    }
    //UE_LOG(LogFMBWeaponItemBoxWidget, Error, TEXT("%i"), WeaponUIItemsBox->HasAnyChildren()) ;
}

void UFMBWeaponItemBoxWidget::OnWeaponSelected(const FWeaponUIData& Data)
{
    for (const auto WeaponUIDataWidget : WeaponUIDataWidgets)
    {
        if (WeaponUIDataWidget)
        {
            const auto IsSelected = Data.WeaponName == WeaponUIDataWidget->GetWeaponUIData().WeaponName;
            WeaponUIDataWidget->WeaponIsSelected(IsSelected);
        }
    }
}

UFMBWeaponComponent* UFMBWeaponItemBoxWidget::GetWeaponComponent() const
{
    const auto WeaponComponent = FMBUtils::GetFMBPlayerComponent<UFMBWeaponComponent>(GetOwningPlayerPawn());
    return WeaponComponent;
}*/