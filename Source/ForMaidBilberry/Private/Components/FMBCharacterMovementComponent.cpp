// For Maid Bilberry Game. All Rights Recerved


#include "Components/FMBCharacterMovementComponent.h"
#include "Player/FMBBaseCharacter.h"

float UFMBCharacterMovementComponent::GetMaxSpeed() const
{
    const auto MaxSpeed = Super::GetMaxSpeed();
    const auto Player = Cast<AFMBBaseCharacter>(GetPawnOwner());
    return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}