// For Maid Bilberry Game. All Rights Reserved

#include "UI/FMBPlayerWidget.h"
#include "UI/FMBItemsBoxWidget.h"
#include "FMBPlayerCharacter.h"
#include "Components/FMBHealthComponent.h"
#include "Components/FMBStaminaComponent.h"
#include "FMBUtils.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Spacer.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBPlayerHUDWidget, All, All)

void UFMBPlayerWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UFMBPlayerWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
    InitItemVerticalBoxes();
}

void UFMBPlayerWidget::OnNewPawn(APawn* NewPawn)
{
    const auto HealthComponent{FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(NewPawn)};
    if (HealthComponent)
    {
        HealthComponent->OnHealthChange.AddUObject(this, &UFMBPlayerWidget::OnHealthChange);
    }
}

void UFMBPlayerWidget::InitItemVerticalBoxes() const
{
    if (!ItemsVerticalBox) return;
    ItemsVerticalBox->ClearChildren();

    const auto SetWidgetHorizontalAlignment = [](UPanelSlot* PanelSlot /*, const EHorizontalAlignment HorizontalAlignment*/)
    {
        if (const auto VertSlot = Cast<UVerticalBoxSlot>(PanelSlot))
        {
            VertSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Right);
        }
    };
    SetWidgetHorizontalAlignment(ItemsVerticalBox->AddChild(CreateItemBoxWidget(EItemType::EIT_PickUp)));
    SetWidgetHorizontalAlignment(ItemsVerticalBox->AddChild(CreateSpacer(FVector2D{0.0f, 15.0f})));
    SetWidgetHorizontalAlignment(ItemsVerticalBox->AddChild(CreateItemBoxWidget(EItemType::EIT_Weapon)));
}

UFMBItemsBoxWidget* UFMBPlayerWidget::CreateItemBoxWidget(const EItemType ItemBoxType) const
{
    const auto ItemBoxWidget{CreateWidget<UFMBItemsBoxWidget>(GetWorld(), ItemBoxWidgetClass)};
    if (!ItemBoxWidget) return nullptr;

    ItemBoxWidget->FillItemIcons(ItemBoxType);
    ItemBoxWidget->SetWidgetItemType(ItemBoxType);
    ItemBoxWidget->SetItemsIconSize(ItemBoxType);
    return ItemBoxWidget;
}

USpacer* UFMBPlayerWidget::CreateSpacer(const FVector2D& SpacerSize) const
{
    if (!WidgetTree) return nullptr;
    const auto SpacerWidget = WidgetTree->ConstructWidget<USpacer>(USpacer::StaticClass());
    if (!SpacerWidget) return nullptr;

    SpacerWidget->SetSize(SpacerSize);
    return SpacerWidget;
}

void UFMBPlayerWidget::OnHealthChange(float Health, float HealthDelta)
{
    if (HealthDelta < 0.0f)
    {
        // OnTakeDamage();
        if (!IsAnimationPlaying(BloodyScreenAnim))
        {
            PlayAnimation(BloodyScreenAnim);
        }
    }
}

float UFMBPlayerWidget::GetHealthPercent() const
{
    const auto HealthComponent{FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(GetOwningPlayerPawn())};
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}

float UFMBPlayerWidget::GetStaminaPercent() const
{
    const auto StaminaComponent{FMBUtils::GetFMBPlayerComponent<UFMBStaminaComponent>(GetOwningPlayerPawn())};
    if (!StaminaComponent) return 0.0f;

    return StaminaComponent->GetStaminaPercent();
}

ESlateVisibility UFMBPlayerWidget::IsPlayerAlive() const
{
    const auto HealthComponent{FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(GetOwningPlayerPawn())};
    return HealthComponent && !HealthComponent->IsDead() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
}

ESlateVisibility UFMBPlayerWidget::IsPlayerSpectating() const
{
    return GetOwningPlayer() && GetOwningPlayer()->GetStateName() == NAME_Spectating ? ESlateVisibility::Visible
                                                                                     : ESlateVisibility::Collapsed;
}
