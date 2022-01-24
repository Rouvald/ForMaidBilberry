// For Maid Bilberry Game. All Rights Recerved

#include "Animation/FMBSwordTrailAnimNotifyState.h"
#include "FMBUtils.h"
#include "FMBWeaponComponent.h"
#include "Player/FMBBaseCharacter.h"
#include "Weapon/FMBBaseWeapon.h"
#include "NiagaraComponent.h"

void UFMBSwordTrailAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration);

    const auto SwordTrailFXComponent = GetSwordTrailFXComponent(MeshComp);
    if (!SwordTrailFXComponent) return;

    SwordTrailFXComponent->SetVisibility(true);
}

void UFMBSwordTrailAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::NotifyEnd(MeshComp, Animation);

    const auto SwordTrailFXComponent = GetSwordTrailFXComponent(MeshComp);
    if (!SwordTrailFXComponent) return;

    SwordTrailFXComponent->SetVisibility(false);
}

UNiagaraComponent* UFMBSwordTrailAnimNotifyState::GetSwordTrailFXComponent(USkeletalMeshComponent* MeshComp) const
{
    const auto Character = Cast<AFMBBaseCharacter>(MeshComp->GetOwner());
    if (!Character || Character->GetMesh() != MeshComp) return nullptr;

    const auto WeaponComponent = FMBUtils::GetFMBPlayerComponent<UFMBWeaponComponent>(MeshComp->GetOwner());
    if (!WeaponComponent) return nullptr;

    const auto CurrentWeapon = WeaponComponent->GetCurrentWeapon();
    if (!CurrentWeapon) return nullptr;

    const auto SwordTrailFXComponent = CurrentWeapon->GetSwordTrailFXComponent();
    if (!SwordTrailFXComponent) return nullptr;

    return SwordTrailFXComponent;
}
