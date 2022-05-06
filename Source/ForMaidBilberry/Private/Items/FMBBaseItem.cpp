// For Maid Bilberry Game. All Rights Reserved

#include "Items/FMBBaseItem.h"

#include "DrawDebugHelpers.h"
#include "FMBItemInfoWidget.h"
#include "FMBItemInteractionComponent.h"
#include "FMBPlayerCharacter.h"
#include "FMBUtils.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBBaseItem, All, All)

AFMBBaseItem::AFMBBaseItem()
{
    PrimaryActorTick.bCanEverTick = false;

    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
    SetRootComponent(ItemMesh);
    ItemMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

    AreaCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AreaCollision"));
    AreaCollision->SetupAttachment(GetRootComponent());
    AreaCollision->SetSphereRadius(500.0f);

    ItemInfoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("ItemInfoWidgetComponent");
    ItemInfoWidgetComponent->SetupAttachment(GetRootComponent());
    ItemInfoWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    ItemInfoWidgetComponent->SetDrawAtDesiredSize(true);
    ItemInfoWidgetComponent->SetRelativeLocation(FVector{0.0f, 0.0f, 0.0f});
}

void AFMBBaseItem::BeginPlay()
{
    Super::BeginPlay();
    checkf(ItemMesh, TEXT("WeaponMesh == nullptr"));
    checkf(AreaCollision, TEXT("AreaCollision == nullptr"));
    checkf(ItemInfoWidgetComponent, TEXT("ItemInfoWidgetComponent == nullptr"));

    ItemInfoWidgetComponent->SetVisibility(false);

    AreaCollision->OnComponentBeginOverlap.AddDynamic(this, &AFMBBaseItem::OnAreaBeginOverlap);
    AreaCollision->OnComponentEndOverlap.AddDynamic(this, &AFMBBaseItem::OnAreaEndOverlap);
    ItemMesh->OnComponentHit.AddDynamic(this, &AFMBBaseItem::FallingHit);

    OnItemStateChanged.AddUObject(this, &AFMBBaseItem::SetItemState);

    ItemMesh->SetMassOverrideInKg(NAME_None, 50.0f);

    SetItemInfo();
    FillItemPropertiesMap();
    SetItemState(DefaultItemState);
    // UE_LOG(LogFMBBaseItem, Display, TEXT("%s: CurrentItemState: %s"), *GetName(), *UEnum::GetValueAsString(CurrentItemState));
}

void AFMBBaseItem::ChangeItemCount(const bool bIsIncrease)
{
    // Item count can be < 0.
    if (ItemData.ItemCount < 1) return;

    bIsIncrease ? ++ItemData.ItemCount : --ItemData.ItemCount;
}

void AFMBBaseItem::OnAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor) return;

    const auto OtherPlayerCharacter = Cast<AFMBPlayerCharacter>(OtherActor);
    if (!OtherPlayerCharacter) return;
    const auto ItemInteractionComponent = OtherPlayerCharacter->FindComponentByClass<UFMBItemInteractionComponent>();
    if (!ItemInteractionComponent) return;

    ItemInteractionComponent->OnItemAreaOverlap.Broadcast(this, true);
}

void AFMBBaseItem::OnAreaEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!OtherActor) return;

    const auto OtherPlayerCharacter = Cast<AFMBPlayerCharacter>(OtherActor);
    if (!OtherPlayerCharacter) return;
    const auto ItemInteractionComponent = OtherPlayerCharacter->FindComponentByClass<UFMBItemInteractionComponent>();
    if (!ItemInteractionComponent) return;

    ItemInteractionComponent->OnItemAreaOverlap.Broadcast(this, false);
    ItemInfoWidgetComponent->SetVisibility(false);
}

void AFMBBaseItem::SetItemInfo() const
{
    const auto ItemInfoWidget{Cast<UFMBItemInfoWidget>(ItemInfoWidgetComponent->GetWidget())};
    if (ItemInfoWidget)
    {
        ItemInfoWidget->SetItemName(ItemData.ItemName);
        ItemInfoWidget->SetItemImage(ItemData.ItemIcon);
    }
}

void AFMBBaseItem::SetItemState(const EItemState NewItemState)
{
    if (CurrentItemState == NewItemState) return;

    CurrentItemState = NewItemState;
    SetItemProperties(NewItemState);
    // UE_LOG(LogFMBBaseItem, Warning, TEXT("%s: CurrentItemState: %s"), *GetName(), *UEnum::GetValueAsString(CurrentItemState));
}

void AFMBBaseItem::SetItemInfoWidgetVisibility(const AFMBPlayerCharacter* CurrentPlayerCharacter, bool bIsVisible) const
{
    ItemInfoWidgetComponent->SetVisibility(bIsVisible);
    UpdateItemInfoProperty(CurrentPlayerCharacter);
}

void AFMBBaseItem::UpdateItemInfoProperty(const AFMBPlayerCharacter* CurrentPlayerCharacter) const
{
    if (!CurrentPlayerCharacter) return;
    const auto ItemInfoWidget{Cast<UFMBItemInfoWidget>(ItemInfoWidgetComponent->GetWidget())};
    if (ItemInfoWidget)
    {
        ItemInfoWidget->SetItemProperty(0.0f);
    }
}

void AFMBBaseItem::FillItemPropertiesMap()
{
    /* @todo: Future refactoring */
    // clang-format off
    ItemStatePropertiesMap.Add(EItemState::EIS_Equipped, FItemStateProperties{
        false,
        false,
        true,
        false,
        ECollisionResponse::ECR_Ignore,
        ECollisionChannel::ECC_WorldStatic,
        ECollisionResponse::ECR_Ignore,
        ECollisionEnabled::NoCollision,
        ECollisionResponse::ECR_Ignore,
        ECollisionEnabled::NoCollision
    });

    ItemStatePropertiesMap.Add(EItemState::EIS_PickUp, FItemStateProperties{
        false,
        false,
        true,
        false,
        ECollisionResponse::ECR_Ignore,
        ECollisionChannel::ECC_Visibility,
        ECollisionResponse::ECR_Block,
        ECollisionEnabled::QueryAndPhysics,
        ECollisionResponse::ECR_Overlap,
        ECollisionEnabled::QueryOnly
    });

    ItemStatePropertiesMap.Add(EItemState::EIS_PickedUp, FItemStateProperties{
        false,
        false,
        false,
        false,
        ECollisionResponse::ECR_Ignore,
        ECollisionChannel::ECC_WorldStatic,
        ECollisionResponse::ECR_Ignore,
        ECollisionEnabled::NoCollision,
        ECollisionResponse::ECR_Ignore,
        ECollisionEnabled::NoCollision
    });

    ItemStatePropertiesMap.Add(EItemState::EIS_Falling, FItemStateProperties{
        true,
        true,
        true,
        true,
        ECollisionResponse::ECR_Ignore,
        ECollisionChannel::ECC_WorldStatic,
        ECollisionResponse::ECR_Block,
        ECollisionEnabled::QueryAndPhysics,
        ECollisionResponse::ECR_Ignore,
        ECollisionEnabled::NoCollision
    });

    /*ItemPropertiesMap.Add(EItemState::EIS_EquipInProgress,
        FItemProperties{false,
                        false,
                        true,
                        ECollisionResponse::ECR_Ignore,
                        ECollisionChannel::ECC_WorldStatic,
                        ECollisionResponse::ECR_Ignore,
                        ECollisionEnabled::NoCollision,
                        ECollisionResponse::ECR_Overlap,
                        ECollisionEnabled::NoCollision,
                        ECollisionResponse::ECR_Ignore,
                        ECollisionEnabled::NoCollision,
                        ECollisionChannel::ECC_Visibility,
                        ECollisionResponse::ECR_Ignore,
        });*/
    //clang-format on
}

void AFMBBaseItem::SetItemProperties(const EItemState NewItemState) const
{
    if (!ItemStatePropertiesMap.Contains(NewItemState)) return;

    // Item Mesh
    ItemMesh->SetSimulatePhysics(ItemStatePropertiesMap[NewItemState].bIsSimulatedPhysics);
    ItemMesh->SetEnableGravity(ItemStatePropertiesMap[NewItemState].bIsGravityEnable);
    ItemMesh->SetVisibility(ItemStatePropertiesMap[NewItemState].bIsVisible);
    ItemMesh->SetNotifyRigidBodyCollision(ItemStatePropertiesMap[NewItemState].bIsSimulationHitEvents);
    ItemMesh->SetCollisionResponseToAllChannels(ItemStatePropertiesMap[NewItemState].ItemMeshCollisionResponseToAllChannels);
    ItemMesh->SetCollisionResponseToChannel(ItemStatePropertiesMap[NewItemState].ItemMeshCollisionChannel,
        ItemStatePropertiesMap[NewItemState].ItemMeshCollisionResponseToChannel);
    ItemMesh->SetCollisionEnabled(ItemStatePropertiesMap[NewItemState].ItemMeshCollisionEnabled);
    //
    // Area Collision
    AreaCollision->SetCollisionResponseToAllChannels(ItemStatePropertiesMap[NewItemState].AreaCollisionResponseToAllChannels);
    AreaCollision->SetCollisionEnabled(ItemStatePropertiesMap[NewItemState].AreaCollisionEnabled);
    //
}

void AFMBBaseItem::ThrowWeapon()
{
    const auto BaseCharacter {GetBaseCharacter()};
    if (!BaseCharacter) return;

    const FRotator MeshRotation{0.0f, ItemMesh->GetComponentRotation().Yaw, 0.0f};
    ItemMesh->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

    FVector TraceStart{}, TraceEnd{};
    if (!FMBUtils::GetTraceData(BaseCharacter, TraceStart, TraceEnd, 80.0f)) return;

    SetActorLocation(TraceEnd);

    auto ThrowingDirection{BaseCharacter->GetActorForwardVector()};
    ThrowingDirection *= 30000.0f;

    ItemMesh->AddImpulse(ThrowingDirection);

    bIsWeaponFalling = true;
}

void AFMBBaseItem::FallingHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (GetWorld())
    {
        if (!GetWorldTimerManager().IsTimerActive(ThrowingTimerHandle))
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), FallingHitSound, GetActorLocation());
            GetWorldTimerManager().SetTimer(ThrowingTimerHandle, this, &AFMBBaseItem::StopFalling, WeaponFallingTime, true);
        }
    }
}

void AFMBBaseItem::StopFalling()
{
    if (!GetWorld() || CurrentItemState != EItemState::EIS_Falling) return;
    WeaponFallingTimeCounter += GetWorldTimerManager().GetTimerElapsed(ThrowingTimerHandle);

    if (WeaponFallingTimeCounter <= MaxWeaponFallingTime && !GetVelocity().IsZero()) return;

    bIsWeaponFalling = false;
    OnItemStateChanged.Broadcast(EItemState::EIS_PickUp);

    if (GetWorld())
    {
        GetWorldTimerManager().ClearTimer(ThrowingTimerHandle);
    }
    WeaponFallingTimeCounter = 0.0f;
}

AFMBBaseCharacter* AFMBBaseItem::GetBaseCharacter() const
{
    return Cast<AFMBBaseCharacter>(GetOwner());
}

AController* AFMBBaseItem::GetController() const
{
    const auto OwnerPawn = Cast<APawn>(GetOwner());
    return OwnerPawn ? OwnerPawn->GetController() : nullptr;
}
