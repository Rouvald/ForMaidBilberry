// For Maid Bilberry Game. All Rights Recerved

#include "PickUps/FMBBasePickUp.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "NiagaraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(PickUpLog, All, All)

AFMBBasePickUp::AFMBBasePickUp()
{
    PrimaryActorTick.bCanEverTick = true;

    /*CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->InitSphereRadius(40.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(CollisionComponent);*/
}

void AFMBBasePickUp::Tick(float DeltaTick)
{
    Super::Tick(DeltaTick);

    // AddActorWorldRotation(FRotator(0.0f, RotationYaw, 0.0f));
}

void AFMBBasePickUp::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    /*const auto Pawn = Cast<APawn>(OtherActor);
    if (GivePickUpTo(Pawn))
    {
        PickUpWasTaken();
    }*/
}

bool AFMBBasePickUp::GivePickUpTo(APawn* Pawn)
{
    return false;
}

/*void AFMBBasePickUp::PickUpWasTaken()
{
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    if (GetRootComponent())
    {
        GetRootComponent()->SetVisibility(false, true);
    }

    if (WantsToRespawn)
    {
        FTimerHandle RespawnPickUpTimerHandle;

        GetWorldTimerManager().SetTimer(RespawnPickUpTimerHandle, this, &AFMBBasePickUp::Respawn, RespawnTime);
    }

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), TakenPickUpSound, GetActorLocation());
}

void AFMBBasePickUp::Respawn()
{
    if (GetRootComponent())
    {
        GetRootComponent()->SetVisibility(true, true);
    }
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}*/