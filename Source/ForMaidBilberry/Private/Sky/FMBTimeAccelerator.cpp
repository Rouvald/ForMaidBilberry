// For Maid Bilberry Game. All Rights Recerved

#include "Sky/FMBTimeAccelerator.h"

AFMBTimeAccelerator::AFMBTimeAccelerator()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AFMBTimeAccelerator::BeginPlay()
{
    Super::BeginPlay();
}

/*void AFMBTimeAccelerator::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent*
OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AFMBTimeAccelerator::OnActorEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}*/