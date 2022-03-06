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
        MovementDirection = GetMovementDirection();

        bIsFalling = BaseCharacter->GetCharacterMovement()->IsFalling();
        bIsRunning = BaseCharacter->IsRunning();

        const auto WeaponComponent = BaseCharacter->FindComponentByClass<UFMBWeaponComponent>();
        if (WeaponComponent)
        {
            bIsBlocking = WeaponComponent->GetIsBlocking();
            bIsCurrentWeapon = WeaponComponent->GetCurrentWeapon() ? true : false;
            CurrentWeaponAnimData = WeaponComponent->GetCurrentWeaponAnimationsData();
            if (WeaponComponent->GetCurrentWeapon())
            {
                CurrentWeaponType = WeaponComponent->GetCurrentWeapon()->GetWeaponType();
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