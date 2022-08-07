// For Maid Bilberry Game. All Rights Reserved

#include "Dev/FMBDevDamageActor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AFMBDevDamageActor::AFMBDevDamageActor()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);
}

void AFMBDevDamageActor::BeginPlay()
{
    Super::BeginPlay();
}

void AFMBDevDamageActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 32, SphereColor);
    UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius,  //
        nullptr, {}, this, nullptr, bDoFullDamage);
}
