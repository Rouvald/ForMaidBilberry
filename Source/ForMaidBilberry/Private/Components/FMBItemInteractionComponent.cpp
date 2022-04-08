// For Maid Bilberry Game. All Rights Reserved

#include "Components/FMBItemInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "FMBBaseItem.h"
#include "FMBPlayerCharacter.h"
#include "FMBUtils.h"
#include "Components/WidgetComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBItemInteractionComponent, All, All)

UFMBItemInteractionComponent::UFMBItemInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UFMBItemInteractionComponent::BeginPlay()
{
    Super::BeginPlay();
    PlayerCharacter = GetPlayerCharacter();

    OnItemAreaOverlap.AddUObject(this, &UFMBItemInteractionComponent::ItemInfoVisibilityTimer);
}

void UFMBItemInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFMBItemInteractionComponent::ItemInfoVisibilityTimer(const AFMBBaseItem* Item, bool bIsOverlap)
{
    const FTimerDelegate ItemTimerDelegate = FTimerDelegate::CreateUObject(this, &UFMBItemInteractionComponent::UpdateItemInfoVisibility);
    if (bIsOverlap)
    {
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().SetTimer(ItemInfoVisibilityTimerHandle, ItemTimerDelegate, 0.1f, true);
            HittedItems.AddUnique(Item);
        }
    }
    else
    {
        HittedItems.RemoveSingle(Item);
        if (HittedItems.Num() == 0 && GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(ItemInfoVisibilityTimerHandle);
        }
    }
    // UE_LOG(LogFMBItemInteractionComponent, Display, TEXT("HittedItems: %d"), HittedItems.Num());
}
void UFMBItemInteractionComponent::UpdateItemInfoVisibility()
{
    FHitResult HitItemResult;

    if (!MakeHitItemVisibility(HitItemResult)) return;

    HitItem = Cast<AFMBBaseItem>(HitItemResult.GetActor());
    if (!HitItem)
    {
        HideAllHittedItems();
        return;
    }

    const auto bIsContains = HittedItems.Contains(HitItem);

    const auto ItemInfo = HitItem->FindComponentByClass<UWidgetComponent>();
    if (ItemInfo)
    {
        bIsContains ? ItemInfo->SetVisibility(true) : ItemInfo->SetVisibility(false);
    }
}
bool UFMBItemInteractionComponent::MakeHitItemVisibility(FHitResult& HitResult) const
{
    if (!GetWorld()) return false;

    FVector TraceStart, TraceEnd;
    if (!FMBUtils::GetTraceData(PlayerCharacter, TraceStart, TraceEnd, ItemVisibilityTraceDistance)) return false;

    FCollisionQueryParams CollisionQueryParams;
    CollisionQueryParams.AddIgnoredActor(GetOwner());

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);
    // DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f);

    return HitResult.bBlockingHit;
}
void UFMBItemInteractionComponent::HideAllHittedItems() const
{
    for (const auto HittedItem : HittedItems)
    {
        const auto ItemInfo = HittedItem->FindComponentByClass<UWidgetComponent>();
        if (ItemInfo)
        {
            ItemInfo->SetVisibility(false);
        }
    }
}

AFMBPlayerCharacter* UFMBItemInteractionComponent::GetPlayerCharacter() const
{
    return Cast<AFMBPlayerCharacter>(GetOwner());
}