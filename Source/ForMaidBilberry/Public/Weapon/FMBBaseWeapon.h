// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FMBBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class FORMAIDBILBERRY_API AFMBBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    AFMBBaseWeapon();

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    USkeletalMeshComponent* WeaponMeshComponent;
    
    virtual void BeginPlay() override;
};
