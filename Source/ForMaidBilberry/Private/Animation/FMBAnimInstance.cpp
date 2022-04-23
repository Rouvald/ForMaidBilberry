// For Maid Bilberry Game. All Rights Reserved

#include "Animation/FMBAnimInstance.h"
#include "FMBBaseCharacter.h"
#include "FMBPlayerWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapon/FMBBaseWeapon.h"

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
        MovementDirection = GetMovementDirection();

        bIsFalling = BaseCharacter->GetCharacterMovement()->IsFalling();
        bIsRunning = BaseCharacter->IsRunning();

        const auto WeaponComponent = BaseCharacter->FindComponentByClass<UFMBBaseWeaponComponent>();
        if (WeaponComponent)
        {
            if (const auto PlayerWeaponComponent{Cast<UFMBPlayerWeaponComponent>(WeaponComponent)})
            {
                bIsBlocking = PlayerWeaponComponent->GetIsBlocking();
            }
            bIsCurrentWeapon = WeaponComponent->GetCurrentWeapon() ? true : false;
            // CurrentWeaponAnimData = WeaponComponent->GetCurrentWeaponAnimationsData();
            if (WeaponComponent->GetCurrentWeapon())
            {
                CurrentWeaponType = WeaponComponent->GetCurrentWeapon()->GetWeaponType();
            }
            else
            {
                CurrentWeaponType = EWeaponType::EWT_NoWeapon;
            }
        }
    }
}

float UFMBAnimInstance::GetMovementDirection() const
{
    if (!BaseCharacter || BaseCharacter->GetVelocity().IsZero()) return 0.0f;

    const auto VelocityNormal = BaseCharacter->GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(BaseCharacter->GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(BaseCharacter->GetActorForwardVector(), VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}
