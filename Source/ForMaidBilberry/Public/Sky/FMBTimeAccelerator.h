// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FMBTimeAccelerator.generated.h"

UCLASS()
class FORMAIDBILBERRY_API AFMBTimeAccelerator : public AActor
{
    GENERATED_BODY()

public:
    AFMBTimeAccelerator();

protected:
    virtual void BeginPlay() override;

    /*private:
        UFUNCTION()
        void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

        UFUNCTION()
        void OnActorEndOverlap(
            UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/
};
