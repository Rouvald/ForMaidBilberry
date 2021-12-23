// For Maid Bilberry Game. All Rights Recerved

#include "Weapon/Components/FMBWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

UFMBWeaponFXComponent::UFMBWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UFMBWeaponFXComponent::PlayImpactFX(FHitResult& HitResult)
{
    auto ImpactData = DefaultImpactData;

    if (HitResult.PhysMaterial.IsValid())
    {
        const auto PhysMat = HitResult.PhysMaterial.Get();
        if (ImpactDataMap.Contains(PhysMat))
        {
            ImpactData = ImpactDataMap[PhysMat];
        }
    }
    // Niagara
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        GetWorld(), ImpactData.NiagaraEffect, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());

    // Sound
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactData.ImpactSound, HitResult.ImpactPoint);
}
