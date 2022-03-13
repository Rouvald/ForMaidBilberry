// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Player/FMBBaseCharacter.h"
#include "FMBAIBaseCharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;
class UFMBBaseWeaponComponent;
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
    UBehaviorTree* BehaviorTreeAsset{nullptr};

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UFMBBaseWeaponComponent* WeaponComponent{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
    UWidgetComponent* HealthBarWidgetComponent{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UWidgetComponent* EnemySignWidgetComponent{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float HealthVisibleDistance{1000.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float EnemySignVisibleDistance{4000.0f};

    virtual void BeginPlay() override;
    virtual void OnDeath() override;

    virtual void OnHealthChange(float Health, float HealthDelta) override;

private:
    FTimerHandle WidgetsVisibilityTimerHandle;

    void UpdateWidgetsVisibility() const;
};
