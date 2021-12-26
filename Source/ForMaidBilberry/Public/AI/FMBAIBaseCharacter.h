// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Player/FMBBaseCharacter.h"
#include "FMBAIBaseCharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API AFMBAIBaseCharacter : public AFMBBaseCharacter
{
    GENERATED_BODY()

public:
    AFMBAIBaseCharacter(const FObjectInitializer& ObjInit);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
    UWidgetComponent* HealthBarWidgetComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float HealthVisibleDistance = 1000.0f;

    virtual void BeginPlay() override;
    virtual void OnDeath() override;
    ;
    virtual void OnHealthChange(float Health, float HealthDelta) override;

private:
    FTimerHandle HealthBarVisibleTimerHandle;

    void UpdateHealthBarWidgetVisible() const;
};
