// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FMBAIController.generated.h"

class UFMBAIPerceptionComponent;
class UFMBRespawnComponent;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API AFMBAIController : public AAIController
{
    GENERATED_BODY()

public:
    AFMBAIController();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UFMBAIPerceptionComponent* FMBAIPerceptionComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Component")
    UFMBRespawnComponent* RespawnComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
    FName FocusOnEnemyKeyName = "EnemyActor";

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
    FName StartAILocationKeyName = "StartBotLocation";

    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaSeconds) override;

private:
    AActor* GetFocusOnActor() const;
};
