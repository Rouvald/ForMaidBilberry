// For Maid Bilberry Game. All Rights Reserved

#include "UI/FMBItemsBoxWidget.h"
#include "FMBUtils.h"
#include "Components/FMBPlayerWeaponComponent.h"
#include "UI/FMBItemIconWidget.h"
#include "Components/HorizontalBox.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBItemBoxWidget, All, All)

void UFMBItemsBoxWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UFMBItemsBoxWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
    // InitItemIcons();
}

void UFMBItemsBoxWidget::OnNewPawn(APawn* Pawn)
{
    PlayerWeaponComponent = FMBUtils::GetFMBPlayerComponent<UFMBPlayerWeaponComponent>(Pawn);
    if (PlayerWeaponComponent)
    {
        PlayerWeaponComponent->OnItemPickedUp.AddUObject(this, &UFMBItemsBoxWidget::OnItemPickedUp);
        PlayerWeaponComponent->OnItemSelected.AddUObject(this, &UFMBItemsBoxWidget::OnItemSelected);
        PlayerWeaponComponent->OnItemCountChange.AddUObject(this, &UFMBItemsBoxWidget::OnItemCountChange);
        PlayerWeaponComponent->OnItemCountVisibility.AddUObject(this, &UFMBItemsBoxWidget::OnItemCountVisibility);
        PlayerWeaponComponent->OnItemIconVisibility.AddUObject(this, &UFMBItemsBoxWidget::OnItemIconVisibility);
    }
    /*@todo: not good decision (after revival first weapon frame turn off)
     *         main problem that weapon component created and pass early than UI (ItemBoxWidget) ->
     *         -> called OnItemSelected.Broadcast in WeaponComponent don't worked properly ("broadcast in sky")
     */
    OnItemSelected(0, EItemType::EIT_Weapon, true);
}

void UFMBItemsBoxWidget::InitItemsBoxWidget(EItemType ItemType)
{
    FillItemIcons(ItemType);
    SetItemsIconSize(ItemType);
}

void UFMBItemsBoxWidget::FillItemIcons(const EItemType NewItemType)
{
    if (!PlayerWeaponComponent || !ItemIconBox)
    {
        return;
    }
    ItemIconBox->ClearChildren();

    SetWidgetItemType(NewItemType);
    /* Set max item icons, for pickups 1 or PlayerWeaponComponent->GetMaxPickUps() */
    WidgetItemIconsAmount = NewItemType == EItemType::EIT_Weapon ? PlayerWeaponComponent->GetMaxWeapons() : 1;

    ItemIconWidgets.Init(nullptr, WidgetItemIconsAmount);

    for (int8 Index = 0; Index < WidgetItemIconsAmount; ++Index)
    {
        const auto WeaponIconWidget = CreateWidget<UFMBItemIconWidget>(GetWorld(), ItemIconWidgetClass);
        if (!WeaponIconWidget)
        {
            continue;
        }

        WeaponIconWidget->SetVisibleItemImage(false);

        ItemIconBox->AddChild(WeaponIconWidget);
        ItemIconWidgets[Index] = WeaponIconWidget;
    }
    if (NewItemType == EItemType::EIT_Weapon && ItemIconWidgets[0])
    {
        ItemIconWidgets[0]->SetItemIsSelected(true);
    }
}

void UFMBItemsBoxWidget::SetItemsIconSize(const EItemType ItemType)
{
    for (const auto ItemIconWidget : ItemIconWidgets)
    {
        if (ItemIconWidget)
        {
            ItemIconWidget->SetItemIconSize(ItemType);
        }
    }
}

void UFMBItemsBoxWidget::OnItemPickedUp(int8 ItemIndex, const FItemData& Data)
{
    if (Data.ItemType != WidgetItemType)
    {
        return;
    }

    if (ItemIconWidgets[ItemIndex] && ItemIconWidgets.Num() > ItemIndex)
    {
        ItemIconWidgets[ItemIndex]->SetVisibleItemImage(true);
        ItemIconWidgets[ItemIndex]->SetItemImage(Data.ItemIcon);
    }
}

void UFMBItemsBoxWidget::OnItemSelected(int8 ItemIndex, const EItemType ItemType, bool bIsVisible)
{
    if (ItemType != WidgetItemType)
    {
        return;
    }

    if (ItemIconWidgets[ItemIndex] && ItemIconWidgets.Num() > ItemIndex)
    {
        ItemIconWidgets[ItemIndex]->SetItemIsSelected(bIsVisible);
    }
}

void UFMBItemsBoxWidget::OnItemCountChange(int8 ItemIndex, const FItemData& Data)
{
    if (Data.ItemType != WidgetItemType)
    {
        return;
    }

    if (ItemIconWidgets[ItemIndex] && ItemIconWidgets.Num() > ItemIndex)
    {
        ItemIconWidgets[ItemIndex]->SetItemCount(Data.ItemCount);
    }
}

void UFMBItemsBoxWidget::OnItemCountVisibility(int8 ItemIndex, const EItemType ItemType, bool bIsVisible)
{
    if (ItemType != WidgetItemType)
    {
        return;
    }

    if (ItemIconWidgets[ItemIndex] && ItemIconWidgets.Num() > ItemIndex)
    {
        ItemIconWidgets[ItemIndex]->SetItemCountVisible(bIsVisible);
    }
}

void UFMBItemsBoxWidget::OnItemIconVisibility(int8 ItemIndex, const EItemType ItemType, bool bIsVisible)
{
    if (ItemType != WidgetItemType)
    {
        return;
    }

    if (ItemIconWidgets[ItemIndex] && ItemIconWidgets.Num() > ItemIndex)
    {
        // PickUpIconWidgets[PickUpIndex]->ItemIsSelected(true);
        ItemIconWidgets[ItemIndex]->SetVisibleItemImage(bIsVisible);
    }
}