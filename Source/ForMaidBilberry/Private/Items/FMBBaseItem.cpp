// For Maid Bilberry Game. All Rights Reserved

#include "Items/FMBBaseItem.h"

#include "FMBItemInfoWidget.h"
#include "FMBItemInteractionComponent.h"
#include "FMBPlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBBaseItem, All, All)

AFMBBaseItem::AFMBBaseItem()
{
    PrimaryActorTick.bCanEverTick = true;

    /*DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
    RootComponent = DefaultRootComponent;*/

    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
    SetRootComponent(ItemMesh);
    // ItemMesh->SetupAttachment(DefaultRootComponent);
    // ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    BoxCollision->SetupAttachment(GetRootComponent());
    BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

    AreaCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AreaCollision"));
    AreaCollision->SetupAttachment(GetRootComponent());
    AreaCollision->SetSphereRadius(500.0f);

    ItemInfoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("ItemInfoWidgetComponent");
    ItemInfoWidgetComponent->SetupAttachment(GetRootComponent());
    ItemInfoWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    ItemInfoWidgetComponent->SetDrawAtDesiredSize(true);
    ItemInfoWidgetComponent->SetRelativeLocation(FVector{0.0f, 0.0f, 100.0f});
}

void AFMBBaseItem::BeginPlay()
{
    Super::BeginPlay();
    checkf(ItemMesh, TEXT("WeaponMesh == nullptr"));
    checkf(BoxCollision, TEXT("BoxCollision == nullptr"));
    checkf(AreaCollision, TEXT("AreaCollision == nullptr"));
    checkf(ItemInfoWidgetComponent, TEXT("ItemInfoWidgetComponent == nullptr"));

    ItemInfoWidgetComponent->SetVisibility(false);

    AreaCollision->OnComponentBeginOverlap.AddDynamic(this, &AFMBBaseItem::OnAreaBeginOverlap);
    AreaCollision->OnComponentEndOverlap.AddDynamic(this, &AFMBBaseItem::OnAreaEndOverlap);
    OnItemStateChanged.AddUObject(this, &AFMBBaseItem::SetItemState);

    // BoxCollision->OnComponentHit.AddDynamic(this, &AFMBBaseItem::StopFalling);

    SetItemInfo();
    FillItemPropertiesMap();
}

void AFMBBaseItem::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    /*if(bIsRotateYaw)
    {
        UE_LOG(LogFMBBaseItem, Display, TEXT("%s, bIsRotateYaw == true, %f"), *this->GetName(), this->GetActorRotation().Yaw);
        AddActorWorldRotation(FRotator(0.0f, RotationYaw, 0.0f));
    }*/
}

/*void AFMBBaseItem::StartItemInterping()
{
    if (!Character) return;
    BaseCharacter = Character;
    ItemBaseLocation = GetActorLocation();
    bIsItemInterping = true;
    SetItemState(EItemState::EIS_EquipInProgress);

    GetWorldTimerManager().SetTimer(ItemInterpingTimerHandle, this, &AWFBaseItem::FinishItemInterping, ItemZCurveTime);

    /*if (BaseCharacter->GetFollowCamera())
    {
        const float CameraRotationYaw{BaseCharacter->GetFollowCamera()->GetComponentRotation().Yaw};
        const float ItemRotationYaw{GetActorRotation().Yaw};
        DefaultRotationYawOffset = ItemRotationYaw - CameraRotationYaw;
    }#1#
}*/

void AFMBBaseItem::OnAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor) return;

    const auto Character = Cast<AFMBPlayerCharacter>(OtherActor);
    if (!Character) return;
    const auto ItemInteractionComponent = Character->FindComponentByClass<UFMBItemInteractionComponent>();
    if (!ItemInteractionComponent) return;

    ItemInteractionComponent->OnItemAreaOverlap.Broadcast(this, true);
}

void AFMBBaseItem::OnAreaEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!OtherActor) return;

    const auto Character = Cast<AFMBPlayerCharacter>(OtherActor);
    if (!Character) return;
    const auto ItemInteractionComponent = Character->FindComponentByClass<UFMBItemInteractionComponent>();
    if (!ItemInteractionComponent) return;

    ItemInteractionComponent->OnItemAreaOverlap.Broadcast(this, false);
    ItemInfoWidgetComponent->SetVisibility(false);
}

void AFMBBaseItem::SetItemInfo() const
{
    const auto ItemInfoWidget{Cast<UFMBItemInfoWidget>(ItemInfoWidgetComponent->GetWidget())};
    if (ItemInfoWidget)
    {
        ItemInfoWidget->SetItemName(ItemName);
    }
    // OnItemStateChanged.Broadcast(EItemState::EIS_Pickup);
}

void AFMBBaseItem::SetItemState(const EItemState NewItemState)
{
    if (CurrentItemState == NewItemState) return;

    CurrentItemState = NewItemState;
    SetItemProperties(NewItemState);
    UE_LOG(LogFMBBaseItem, Display, TEXT("%s: CurrentItemState: %s"), *GetName(), *UEnum::GetValueAsString(CurrentItemState));
}

void AFMBBaseItem::FillItemPropertiesMap()
{
    /* @todo: Future refactoring */

    ItemPropertiesMap.Add(
        EItemState::EIS_Equipped, FItemProperties{false, false, true, ECollisionResponse::ECR_Ignore, ECollisionChannel::ECC_WorldStatic,
                                      ECollisionResponse::ECR_Ignore, ECollisionEnabled::NoCollision, ECollisionResponse::ECR_Ignore,
                                      ECollisionEnabled::NoCollision, ECollisionResponse::ECR_Ignore, ECollisionEnabled::NoCollision,
                                      ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore});

    ItemPropertiesMap.Add(EItemState::EIS_Pickup, FItemProperties{
                                                      false,
                                                      false,
                                                      true,
                                                      ECollisionResponse::ECR_Ignore,
                                                      ECollisionChannel::ECC_WorldStatic,
                                                      ECollisionResponse::ECR_Ignore,
                                                      ECollisionEnabled::NoCollision,
                                                      ECollisionResponse::ECR_Overlap,
                                                      ECollisionEnabled::QueryOnly,
                                                      ECollisionResponse::ECR_Ignore,
                                                      ECollisionEnabled::QueryAndPhysics,
                                                      ECollisionChannel::ECC_Visibility,
                                                      ECollisionResponse::ECR_Block,
                                                  });

    ItemPropertiesMap.Add(
        EItemState::EIS_Falling, FItemProperties{true, true, true, ECollisionResponse::ECR_Ignore, ECollisionChannel::ECC_WorldStatic,
                                     ECollisionResponse::ECR_Block, ECollisionEnabled::QueryAndPhysics, ECollisionResponse::ECR_Ignore,
                                     ECollisionEnabled::NoCollision, ECollisionResponse::ECR_Ignore, ECollisionEnabled::NoCollision,
                                     ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore});

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
}

void AFMBBaseItem::SetItemProperties(const EItemState NewItemState) const
{
    if (!ItemPropertiesMap.Contains(NewItemState)) return;

    // Item Mesh
    ItemMesh->SetSimulatePhysics(ItemPropertiesMap[NewItemState].bIsSimulatedPhysics);
    ItemMesh->SetEnableGravity(ItemPropertiesMap[NewItemState].bIsGravityEnable);
    ItemMesh->SetVisibility(ItemPropertiesMap[NewItemState].bIsVisible);
    ItemMesh->SetCollisionResponseToAllChannels(ItemPropertiesMap[NewItemState].ItemMeshCollisionResponseToAllChannels);
    ItemMesh->SetCollisionResponseToChannel(
        ItemPropertiesMap[NewItemState].ItemMeshCollisionChannel, ItemPropertiesMap[NewItemState].ItemMeshCollisionResponseToChannel);
    ItemMesh->SetCollisionEnabled(ItemPropertiesMap[NewItemState].ItemMeshCollisionEnabled);
    //
    // Area Collision
    AreaCollision->SetCollisionResponseToAllChannels(ItemPropertiesMap[NewItemState].AreaCollisionResponseToAllChannels);
    AreaCollision->SetCollisionEnabled(ItemPropertiesMap[NewItemState].AreaCollisionEnabled);
    //
    // Box Collision
    BoxCollision->SetCollisionResponseToAllChannels(ItemPropertiesMap[NewItemState].BoxCollisionResponseToAllChannels);
    BoxCollision->SetCollisionResponseToChannel(
        ItemPropertiesMap[NewItemState].BoxCollisionChannel, ItemPropertiesMap[NewItemState].BoxCollisionResponseToChannel);
    BoxCollision->SetCollisionEnabled(ItemPropertiesMap[NewItemState].BoxCollisionEnabled);
    //
}

void AFMBBaseItem::ThrowWeapon()
{
    const FRotator MeshRotation{0.0f, ItemMesh->GetComponentRotation().Yaw, 1.0f};
    ItemMesh->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

    const auto MeshForwardVector{ItemMesh->GetForwardVector()};
    const auto MeshRightVector{ItemMesh->GetRightVector()};
    auto ThrowingDirection{MeshRightVector.RotateAngleAxis(-20.f, MeshForwardVector)};

    const float RandomRotation{FMath::FRandRange(25.0f, 35.0f)};
    ThrowingDirection = ThrowingDirection.RotateAngleAxis(RandomRotation, FVector{0.0f, 0.0f, 1.0f});
    ThrowingDirection *= 5000.0f;

    ItemMesh->AddImpulse(ThrowingDirection);

    bIsWeaponFalling = true;
    GetWorldTimerManager().SetTimer(ThrowingTimerHandle, this, &AFMBBaseItem::StopFalling, WeaponFallingTime, false);
}

void AFMBBaseItem::StopFalling() /*(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector
                                    NormalImpulse, const FHitResult& Hit )*/
{
    bIsWeaponFalling = false;
    if (CurrentItemState == EItemState::EIS_Falling)
    {
        const FRotator MeshRotation{0.0f, ItemMesh->GetComponentRotation().Yaw, 1.0f};
        ItemMesh->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
    }
    OnItemStateChanged.Broadcast(EItemState::EIS_Pickup);
}

AController* AFMBBaseItem::GetController() const
{
    const auto OwnerPawn = Cast<APawn>(GetOwner());
    return OwnerPawn ? OwnerPawn->GetController() : nullptr;
}
