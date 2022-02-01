// For Maid Bilberry Game. All Rights Reserved

#include "Animation/FMBAnimInstance.h"
#include "FMBBaseCharacter.h"
#include "FMBWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UFMBAnimInstance::NativeInitializeAnimation()
{
    BaseCharacter = Cast<AFMBBaseCharacter>(GetOwningActor());
}

void UFMBAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    UpdateAnimationProperties(DeltaSeconds);
}

void UFMBAnimInstance::UpdateAnimationProperties(float DeltaSeconds)
{
    if (!BaseCharacter)
    {
        BaseCharacter = Cast<AFMBBaseCharacter>(GetOwningActor());
    }
    if (BaseCharacter)
    {
        auto Speed{BaseCharacter->GetVelocity()};
        Speed.Z = 0.0f;
        Velocity = Speed.Size();

        bIsFalling = BaseCharacter->GetCharacterMovement()->IsFalling();
        bIsRunning = BaseCharacter->IsRunning();

        const auto WeaponComponent = BaseCharacter->FindComponentByClass<UFMBWeaponComponent>();
        if (WeaponComponent)
        {
            bIsCurrentWeapon = WeaponComponent->GetCurrentWeapon() ? true : false;
            CurrentWeaponAnimData = WeaponComponent->GetCurrentWeaponAnimationsData();
            if (WeaponComponent->GetCurrentWeapon())
            {
                CurrentWeaponType = WeaponComponent->GetCurrentWeapon()->GetWeaponType();
            }
        }
    }
}
