// For Maid Bilberry Game. All Rights Reserved

#include "Dev/FMBDamageSphereComponent.h"
#include "Kismet/GameplayStatics.h"

void UFMBDamageSphereComponent::BeginPlay()
{
    Super::BeginPlay();

    // InitSphereRadius(CurrentSphereRadius);
    // SetRelativeLocation(FVector{0.0f, 0.0f, CurrentSphereRadius});
    SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    OnComponentBeginOverlap.AddDynamic(this, &UFMBDamageSphereComponent::SphereComponentBeginOverlap);
    OnComponentEndOverlap.AddDynamic(this, &UFMBDamageSphereComponent::SphereComponentEndOverlap);
}

void UFMBDamageSphereComponent::SphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (GetWorld())
    {
        const FTimerDelegate ApplyTimerDelegate{FTimerDelegate::CreateUObject(this, &UFMBDamageSphereComponent::ApplyDamage, OtherActor)};
        GetWorld()->GetTimerManager().SetTimer(FireDamageTimerHandle, ApplyTimerDelegate, 0.1f, true);
    }
}

void UFMBDamageSphereComponent::SphereComponentEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(FireDamageTimerHandle);
    }
}

void UFMBDamageSphereComponent::ApplyDamage(AActor* OtherActor)
{
    UGameplayStatics::ApplyPointDamage(OtherActor, FireDamage, FVector::ZeroVector, FHitResult{}, nullptr, GetOwner(), nullptr);
}