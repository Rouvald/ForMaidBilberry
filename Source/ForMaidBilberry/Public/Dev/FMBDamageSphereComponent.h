// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "FMBDamageSphereComponent.generated.h"

/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBDamageSphereComponent : public USphereComponent
{
    GENERATED_BODY()
protected:
    // UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sphere")
    // float CurrentSphereRadius{100.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sphere")
    float FireDamage{1.0f};

    virtual void BeginPlay() override;

private:
    FTimerHandle FireDamageTimerHandle;

    UFUNCTION()
    void SphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void SphereComponentEndOverlap(
        UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void ApplyDamage(AActor* OtherActor);
};
