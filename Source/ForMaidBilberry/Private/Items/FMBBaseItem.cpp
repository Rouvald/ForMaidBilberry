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
    PrimaryActorTick.bCanEverTick = false;

    DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
    RootComponent = DefaultRootComponent;

    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
    ItemMesh->SetupAttachment(DefaultRootComponent);
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

    SetItemInfo();
}

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
    // ItemInfoWidgetComponent->SetVisibility(false);
}

void AFMBBaseItem::SetItemInfo()
{
    const auto ItemInfoWidget{Cast<UFMBItemInfoWidget>(ItemInfoWidgetComponent->GetWidget())};
    if (ItemInfoWidget)
    {
        ItemInfoWidget->SetItemName(ItemName);
    }
    // OnItemStateChanged.Broadcast(EItemState::EIS_Pickup);
}

AController* AFMBBaseItem::GetController() const
{
    const auto OwnerPawn = Cast<APawn>(GetOwner());
    return OwnerPawn ? OwnerPawn->GetController() : nullptr;
}