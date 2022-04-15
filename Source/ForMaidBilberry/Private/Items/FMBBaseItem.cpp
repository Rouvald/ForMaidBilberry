// For Maid Bilberry Game. All Rights Reserved

#include "Items/FMBBaseItem.h"

#include "DrawDebugHelpers.h"
#include "FMBItemInfoWidget.h"
#include "FMBItemInteractionComponent.h"
#include "FMBPlayerCharacter.h"
#include "FMBUtils.h"
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
    // ItemMesh->SetMassOverrideInKg(NAME_None, 50.0f);
    // ItemMesh->SetRelativeLocation(FVector{0.0f, 0.0f,0.0f});
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

    ItemMesh->OnComponentHit.AddDynamic(this, &AFMBBaseItem::StopFalling);
    ItemMesh->SetMassOverrideInKg(NAME_None, 50.0f);

    SetItemInfo();
    FillItemPropertiesMap();
    // UE_LOG(LogFMBBaseItem, Display, TEXT("%s: CurrentItemState: %s"), *GetName(), *UEnum::GetValueAsString(CurrentItemState));
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
        EItemState::EIS_Equipped, FItemProperties{false, false, true, false, ECollisionResponse::ECR_Ignore,
                                      ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore, ECollisionEnabled::NoCollision,
                                      ECollisionResponse::ECR_Ignore, ECollisionEnabled::NoCollision, ECollisionResponse::ECR_Ignore,
                                      ECollisionEnabled::NoCollision, ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore});

    ItemPropertiesMap.Add(EItemState::EIS_Pickup, FItemProperties{
                                                      false,
                                                      false,
                                                      true,
                                                      false,
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

    ItemPropertiesMap.Add(EItemState::EIS_PickedUp, FItemProperties{
                                                        false,
                                                        false,
                                                        false,
                                                        false,
                                                        ECollisionResponse::ECR_Ignore,
                                                        ECollisionChannel::ECC_WorldStatic,
                                                        ECollisionResponse::ECR_Ignore,
                                                        ECollisionEnabled::NoCollision,
                                                        ECollisionResponse::ECR_Ignore,
                                                        ECollisionEnabled::NoCollision,
                                                        ECollisionResponse::ECR_Ignore,
                                                        ECollisionEnabled::NoCollision,
                                                        ECollisionChannel::ECC_Visibility,
                                                        ECollisionResponse::ECR_Ignore,
                                                    });

    ItemPropertiesMap.Add(
        EItemState::EIS_Falling, FItemProperties{true, true, true, true, ECollisionResponse::ECR_Ignore, ECollisionChannel::ECC_WorldStatic,
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
    ItemMesh->SetNotifyRigidBodyCollision(ItemPropertiesMap[NewItemState].bIsSimulationHitEvents);
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
    const auto PlayerCharacter{GetPlayerCharacter()};
    if (!PlayerCharacter) return;

    const FRotator MeshRotation{0.0f, ItemMesh->GetComponentRotation().Yaw, 0.0f};
    ItemMesh->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

    FVector TraceStart{}, TraceEnd{};
    if (!FMBUtils::GetTraceData(PlayerCharacter, TraceStart, TraceEnd, 50.0f)) return;

    SetActorLocation(TraceEnd);

    auto ThrowingDirection{PlayerCharacter->GetActorForwardVector()};
    ThrowingDirection *= 20000.0f;

    ItemMesh->AddImpulse(ThrowingDirection);

    /*FVector TraceStart{}, TraceEnd{};
    if (!FMBUtils::GetTraceData(GetPlayerCharacter(), TraceStart, TraceEnd, 200.0f)) return;

    UE_LOG(LogFMBBaseItem, Display, TEXT("TraceStart: %f, %f, %f ==== TraceEnd: %f, %f, %f"), TraceStart.X, TraceStart.Y, TraceStart.Z,
    TraceEnd.X, TraceEnd.Y, TraceEnd.Z);

    const auto Pawn{Cast<APawn>(GetOwner())};
    if (!Pawn) return;

    SetActorLocation(TraceEnd);
    ItemMesh->AddImpulse(Pawn->GetActorForwardVector()*100.0f);*/

    bIsWeaponFalling = true;
}

void AFMBBaseItem::StopFalling /*()*/ (
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (CurrentItemState != EItemState::EIS_Falling) return;

    bIsWeaponFalling = false;

    const FRotator MeshRotation{0.0f, ItemMesh->GetComponentRotation().Yaw, 1.0f};
    ItemMesh->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

    OnItemStateChanged.Broadcast(EItemState::EIS_Pickup);
}

AFMBPlayerCharacter* AFMBBaseItem::GetPlayerCharacter() const
{
    return Cast<AFMBPlayerCharacter>(GetOwner());
}

AController* AFMBBaseItem::GetController() const
{
    const auto OwnerPawn = Cast<APawn>(GetOwner());
    return OwnerPawn ? OwnerPawn->GetController() : nullptr;
}
