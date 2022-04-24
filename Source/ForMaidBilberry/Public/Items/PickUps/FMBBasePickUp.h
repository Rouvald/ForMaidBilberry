// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "FMBBaseItem.h"
#include "FMBBasePickUp.generated.h"

// class USphereComponent;
class USoundCue;
// class UNiagaraComponent;

UCLASS()
class FORMAIDBILBERRY_API AFMBBasePickUp : public AFMBBaseItem
{
    GENERATED_BODY()

public:
    AFMBBasePickUp();

    void UsePickUp();

    FORCEINLINE EPickUpType GetPickUpType() const { return PickUpType; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PickUp")
    EPickUpType PickUpType{EPickUpType::EPT_HealthFlask};

    /*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PickUp")
    bool WantsToRespawn{false};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PickUp")
    float RespawnTime{5.0f};*/

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* TakenPickUpSound{nullptr};

    virtual bool CanUsePickUp(APawn* Pawn);
    virtual void PickUpWasUsed();
    // virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

    // private:
    // virtual bool GivePickUpTo(APawn* Pawn);

    /*void PickUpWasTaken();
    void Respawn();*/
};
