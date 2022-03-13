// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FMBBaseItem.generated.h"

class UBoxComponent;
class USphereComponent;

UCLASS()
class FORMAIDBILBERRY_API AFMBBaseItem : public AActor
{
    GENERATED_BODY()

public:
    AFMBBaseItem();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
    USceneComponent* DefaultRootComponent{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* BoxCollision{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* AreaCollision{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
    UStaticMeshComponent* ItemMesh{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
    FName ItemName{"BaseItem"};

    virtual void BeginPlay() override;

    AController* GetController() const;
};
