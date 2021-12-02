// For Maid Bilberry Game. All Rights Recerved


#include "Weapon/Components/FMBWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"

UFMBWeaponFXComponent::UFMBWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UFMBWeaponFXComponent::PlayImpactFX(FHitResult& HitResult)
{
    auto Effect = DefaultEffect;

    if (HitResult.PhysMaterial.IsValid())
    {
        const auto PhysMat = HitResult.PhysMaterial.Get();
        if (EffectsMap.Contains(PhysMat))
        {
            Effect = EffectsMap[PhysMat]; 
        }
    }
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
}
