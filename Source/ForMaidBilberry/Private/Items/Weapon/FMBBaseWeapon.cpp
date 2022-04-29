// For Maid Bilberry Game. All Rights Reserved

#include "Weapon/FMBBaseWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Weapon/Components/FMBWeaponFXComponent.h"
#include "FMBCoreTypes.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/FMBItemInfoWidget.h"

DECLARE_LOG_CATEGORY_CLASS(LogFMBBaseWeapon, All, All);

AFMBBaseWeapon::AFMBBaseWeapon()
{
    AreaCollision->SetRelativeLocation(ItemMesh->GetRelativeLocation());

    WeaponFXComponent = CreateDefaultSubobject<UFMBWeaponFXComponent>(TEXT("WeaponFXComponent"));
}

void AFMBBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    checkf(WeaponFXComponent, TEXT("WeaponFxComponent == nullptr"));

    ChooseDamageAmount.Add(EChooseAttack::ECA_FastAttack, WeaponDamageData.DefaultDamage);
    ChooseDamageAmount.Add(EChooseAttack::ECA_StrongAttack, WeaponDamageData.DefaultDamage * WeaponDamageData.StrongAttackModifier);
}

void AFMBBaseWeapon::SetItemInfo() const
{
    Super::SetItemInfo();

    const auto ItemInfoWidget{Cast<UFMBItemInfoWidget>(ItemInfoWidgetComponent->GetWidget())};
    if (ItemInfoWidget)
    {
        ItemInfoWidget->SetItemProperty(WeaponDamageData.DefaultDamage);
    }
}

void AFMBBaseWeapon::MeleeAttack(const EChooseAttack ChooseAttack)
{
    UE_LOG(LogFMBBaseWeapon, Warning, TEXT("Base Weapon can't attack"));
}

void AFMBBaseWeapon::StopDrawTrace()
{
    UE_LOG(LogFMBBaseWeapon, Warning, TEXT("Base Weapon can't stop draw trace"));
}
