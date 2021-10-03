// For Maid Bilberry Game. All Rights Recerved


#include "Weapon/FMBBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

AFMBBaseWeapon::AFMBBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

    WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponComponent");
    SetRootComponent(WeaponMeshComponent);
}

void AFMBBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}
