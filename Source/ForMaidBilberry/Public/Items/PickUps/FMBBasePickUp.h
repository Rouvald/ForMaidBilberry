// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "FMBBaseItem.h"
#include "FMBBasePickUp.generated.h"

class USphereComponent;
class USoundCue;
// class UNiagaraComponent;

UCLASS()
class FORMAIDBILBERRY_API AFMBBasePickUp : public AFMBBaseItem
{
    GENERATED_BODY()

public:
    AFMBBasePickUp();

    virtual void Tick(float DeltaTick) override;

    FORCEINLINE EPickUpType GetPickUpType() const { return PickupType; }

    FORCEINLINE int32 GetItemCount() const { return ItemCount; }
    FORCEINLINE void ChangeItemCount(const bool bIsIncrease) { bIsIncrease ? ++ItemCount : ItemCount = FMath::Max(0, --ItemCount); };

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PickUp")
    EPickUpType PickupType{EPickUpType::EPT_HealthFlask};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PickUp")
    int32 ItemCount{1};

    /*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PickUp")
    bool WantsToRespawn{false};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PickUp")
    float RespawnTime{5.0f};*/

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* TakenPickUpSound{nullptr};

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
    virtual bool GivePickUpTo(APawn* Pawn);

    /*void PickUpWasTaken();
    void Respawn();*/
};
