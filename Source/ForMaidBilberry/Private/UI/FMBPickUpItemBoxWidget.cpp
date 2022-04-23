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

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UFMBPickUpItemBoxWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
    InitPickUpItem();
}

void UFMBPickUpItemBoxWidget::OnNewPawn(APawn* Pawn)
{
    PlayerWeaponComponent = FMBUtils::GetFMBPlayerComponent<UFMBPlayerWeaponComponent>(Pawn);
    if (PlayerWeaponComponent)
    {
        PlayerWeaponComponent->OnItemPickedUp.AddUObject(this, &UFMBPickUpItemBoxWidget::OnPickUpPickedUp);
        PlayerWeaponComponent->OnItemSelected.AddUObject(this, &UFMBPickUpItemBoxWidget::OnPickUpSelected);
        PlayerWeaponComponent->OnItemCountChange.AddUObject(this, &UFMBPickUpItemBoxWidget::OnPickUpCountChange);
        PlayerWeaponComponent->OnItemCountVisible.AddUObject(this, &UFMBPickUpItemBoxWidget::OnPickUpCountVisible);
        PlayerWeaponComponent->OnItemIconVisibility.AddUObject(this, &UFMBPickUpItemBoxWidget::OnPickUpIconVisibility);
    }
}

void UFMBPickUpItemBoxWidget::InitPickUpItem()
{
    if (!PlayerWeaponComponent) return;

    if (!PickUpIconBox) return;
    PickUpIconBox->ClearChildren();

    PickUpIconWidgets.Init(nullptr, PlayerWeaponComponent->GetMaxPickUps());

    for (int8 Index = 0; Index < PlayerWeaponComponent->GetMaxPickUps(); ++Index)
    {
        const auto PickUpIconWidget = CreateWidget<UFMBItemIconWidget>(GetWorld(), PickUpIconWidgetClass);
        if (!PickUpIconWidget) return;

        PickUpIconWidget->SetVisibleItemImage(false);
        PickUpIconWidget->SetItemIconSize(EItemType::EIT_PickUp);

        PickUpIconBox->AddChild(PickUpIconWidget);
        PickUpIconWidgets[Index] = PickUpIconWidget;
    }

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

void UFMBPickUpItemBoxWidget::OnPickUpSelected(int8 PickUpIndex, const EItemType ItemType, bool bIsVisible)
{
    if (ItemType != EItemType::EIT_PickUp) return;

    if (PickUpIconWidgets[PickUpIndex] && PickUpIconWidgets.Num() > PickUpIndex)
    {
        PickUpIconWidgets[PickUpIndex]->SetItemIsSelected(bIsVisible);
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

void UFMBPickUpItemBoxWidget::OnPickUpCountVisible(int8 PickUpIndex, const EItemType ItemType, bool bIsVisible)
{
    if (ItemType != EItemType::EIT_PickUp) return;

    if (PickUpIconWidgets[PickUpIndex] && PickUpIconWidgets.Num() > PickUpIndex)
    {
        PickUpIconWidgets[PickUpIndex]->SetItemCountVisible(bIsVisible);
    }
}

void UFMBPickUpItemBoxWidget::OnPickUpIconVisibility(int8 PickUpIndex, const EItemType ItemType, bool bIsVisible)
{
    if (ItemType != EItemType::EIT_PickUp) return;

    if (PickUpIconWidgets[PickUpIndex] && PickUpIconWidgets.Num() > PickUpIndex)
    {
        // PickUpIconWidgets[PickUpIndex]->ItemIsSelected(true);
        PickUpIconWidgets[PickUpIndex]->SetVisibleItemImage(bIsVisible);
    }
}

/*UFMBPlayerWeaponComponent* UFMBPickUpItemBoxWidget::GetWeaponComponent() const
{
    return FMBUtils::GetFMBPlayerComponent<UFMBPlayerWeaponComponent>(GetOwningPlayerPawn());
}*/
