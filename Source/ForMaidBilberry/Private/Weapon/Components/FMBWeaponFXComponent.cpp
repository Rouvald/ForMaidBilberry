// For Maid Bilberry Game. All Rights Recerved


#include "Weapon/Components/FMBWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"

UFMBWeaponFXComponent::UFMBWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UFMBWeaponFXComponent::PlayImpactFX(FHitResult& Hitresult)
{
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, Hitresult.ImpactPoint, Hitresult.ImpactNormal.Rotation());
}