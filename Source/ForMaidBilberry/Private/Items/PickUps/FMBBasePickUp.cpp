// For Maid Bilberry Game. All Rights Reserved

#include "PickUps/FMBBasePickUp.h"

#include "FMBPlayerCharacter.h"
#include "FMBPlayerWeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Components/WidgetComponent.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBBasePickUp, All, All)

AFMBBasePickUp::AFMBBasePickUp()
{
    PrimaryActorTick.bCanEverTick = false;

    /*BoxCollision->SetRelativeLocation(FVector{0.0f, 0.0f, 12.0f});
    BoxCollision->SetBoxExtent(FVector{11.0f, 11.0f, 13.0f});*/

    AreaCollision->SetRelativeLocation(ItemMesh->GetRelativeLocation());

    ItemInfoWidgetComponent->SetRelativeLocation(FVector{0.0f, 0.0f, 15.0f});

    /*CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->InitSphereRadius(40.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(CollisionComponent);*/
}

void AFMBBasePickUp::UsePickUp()
{
    const auto Pawn{Cast<APawn>(GetOwner())};
    if (CanUsePickUp(Pawn))
    {
        PickUpWasUsed();
    }
}

bool AFMBBasePickUp::CanUsePickUp(APawn* Pawn)
{
    UE_LOG(LogFMBBasePickUp, Warning, TEXT("Did you see the cartoon? Then pay money!"));
    return false;
}

void AFMBBasePickUp::PickUpWasUsed()
{
    const auto PLayerCharacter{GetPlayerCharacter()};
    if (PLayerCharacter)
    {
        const auto WeaponComponent{PLayerCharacter->FindComponentByClass<UFMBPlayerWeaponComponent>()};
        if (WeaponComponent)
        {
            WeaponComponent->PickUpWasUsed();
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickUpWasUsedSound, PLayerCharacter->GetActorLocation());
        }
    }
}

/*void AFMBBasePickUp::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    /*const auto Pawn = Cast<APawn>(OtherActor);
    if (GivePickUpTo(Pawn))
    {
        PickUpWasTaken();
    }#1#
}*/

/*bool AFMBBasePickUp::GivePickUpTo(APawn* Pawn)
{
    return false;
}*/

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
