// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FMBBaseShield.generated.h"

UCLASS()
class FORMAIDBILBERRY_API AFMBBaseShield : public AActor
{
    GENERATED_BODY()

public:
    AFMBBaseShield();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
    USceneComponent* DefaultRootComponent{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
    UStaticMeshComponent* ShieldMesh{nullptr};

    virtual void BeginPlay() override;
};
