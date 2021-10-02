// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FMBDevDamageActor.generated.h"

class USceneComponent;

UCLASS()
class FORMAIDBILBERRY_API AFMBDevDamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFMBDevDamageActor();

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USceneComponent* SceneComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Radius = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FColor SphereColor = FColor::Red;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool DoFullDamage = false;
    
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
