// For Maid Bilberry Game. All Rights Reserved

#include "Environment/FMBFireCamp.h"

AFMBFireCamp::AFMBFireCamp()
{
    PrimaryActorTick.bCanEverTick = true;

    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    SetRootComponent(RootSceneComponent);

    DamageSphereComponent = CreateDefaultSubobject<UFMBDamageSphereComponent>(TEXT("DamageSphereComponent"));
    DamageSphereComponent->SetupAttachment(GetRootComponent());
}
