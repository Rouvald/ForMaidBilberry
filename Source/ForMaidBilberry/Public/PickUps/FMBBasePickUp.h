// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FMBBasePickUp.generated.h"

class USphereComponent;

UCLASS()
class FORMAIDBILBERRY_API AFMBBasePickUp : public AActor
{
    GENERATED_BODY()

public:
    AFMBBasePickUp();

    virtual void Tick(float DeltaTick) override;

protected:
    UPROPERTY(VisibleAnywhere, Category="PickUp")
    USphereComponent* CollisionComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="PickUp")
    float RespawnTime = 3.0f;

    virtual void BeginPlay() override;

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
    float RotationYaw = 1.5f;
    
    virtual bool GivePickUpTo(APawn* Pawn);

    void PickUpWasTaken();
    void Respawn();
};