// For Maid Bilberry Game. All Rights Reserved

#include "Weapon/FMBBaseShield.h"

AFMBBaseShield::AFMBBaseShield()
{
    PrimaryActorTick.bCanEverTick = false;

    DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
    RootComponent = DefaultRootComponent;

    ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    ShieldMesh->SetupAttachment(DefaultRootComponent);
    ShieldMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void AFMBBaseShield::BeginPlay()
{
    Super::BeginPlay();

    checkf(ShieldMesh, TEXT("ShieldMesh == nullptr"));
}
