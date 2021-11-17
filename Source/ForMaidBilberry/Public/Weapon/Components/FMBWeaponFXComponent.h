// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FMBWeaponFXComponent.generated.h"

class UNiagaraSystem;
class UPhysicalMaterial;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FORMAIDBILBERRY_API UFMBWeaponFXComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFMBWeaponFXComponent();

    void PlayImpactFX(FHitResult& HitResult);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
    UNiagaraSystem* DefaultEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
    TMap<UPhysicalMaterial*, UNiagaraSystem*> EffectsMap;
};
