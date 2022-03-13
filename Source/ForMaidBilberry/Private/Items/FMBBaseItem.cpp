// For Maid Bilberry Game. All Rights Reserved

#include "Items/FMBBaseItem.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

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
}

void AFMBBaseItem::BeginPlay()
{
    Super::BeginPlay();
    checkf(ItemMesh, TEXT("WeaponMesh == nullptr"));
    checkf(BoxCollision, TEXT("BoxCollision = nullptr"));
    checkf(AreaCollision, TEXT("AreaCollision = nullptr"));
}

AController* AFMBBaseItem::GetController() const
{
    const auto OwnerPawn = Cast<APawn>(GetOwner());
    return OwnerPawn ? OwnerPawn->GetController() : nullptr;
}