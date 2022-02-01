// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "FMBDamageSphereComponent.h"
#include "GameFramework/Actor.h"
#include "FMBFireCamp.generated.h"

UCLASS()
class FORMAIDBILBERRY_API AFMBFireCamp : public AActor
{
    GENERATED_BODY()

public:
    AFMBFireCamp();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
    USceneComponent* RootSceneComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
    UFMBDamageSphereComponent* DamageSphereComponent;
};
