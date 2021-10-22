// For Maid Bilberry Game. All Rights Recerved

#include "Components/FMBCharacterMovementComponent.h"
#include "Components/FMBHealthComponent.h"
#include "Animation//FMBAnimUtils.h"
#include "Animation/FMBAnimFinishedNotify.h"
#include "Player/FMBBaseCharacter.h"
#include "FMBUtils.h"

DECLARE_LOG_CATEGORY_CLASS(MovementComponentLog, All, All);

void UFMBCharacterMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    InitAnimation();
}

float UFMBCharacterMovementComponent::GetMaxSpeed() const
{
    const auto MaxSpeed = Super::GetMaxSpeed();
    const auto Character = Cast<AFMBBaseCharacter>(GetPawnOwner());
    /*
    if(!(CanRolling()))
    {
        return MaxSpeed * RollingModifier;
    }
    */
    return Character && Character->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}

void UFMBCharacterMovementComponent::Rolling()
{
    const auto Character = Cast<AFMBBaseCharacter>(GetPawnOwner());
    if (!Character) return;

    /*const auto ForwardVector = Character->GetActorLocation();
    if (ForwardVector.IsZero()) return;
    const FVector NewLocation = ForwardVector * FVector(0.0f, 10.0f, 0.0f); 
    Character->SetActorLocation(NewLocation);*/
    
    RollingAnimInProgress = true;
    Character->PlayAnimMontage(RollingAnimMontage);
}


void UFMBCharacterMovementComponent::InitAnimation()
{
    const auto AttackEvent = FMBAnimUtils::FindNotifyByClass<UFMBAnimFinishedNotify>(RollingAnimMontage);
    if (AttackEvent)
    {
        AttackEvent->OnNotify.AddUObject(this, &UFMBCharacterMovementComponent::OnRollingFinished);
    }
    else
    {
        UE_LOG(MovementComponentLog, Error, TEXT("Rolling Finished anim notify don't set"));
        checkNoEntry();
    }
}

void UFMBCharacterMovementComponent::OnRollingFinished(USkeletalMeshComponent* MeshComp)
{
    const auto Character = Cast<AFMBBaseCharacter>(GetPawnOwner());
    if (!Character || Character->GetMesh() != MeshComp) return;

    const auto HealthComponent = FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(GetPawnOwner());
    if (!HealthComponent) return;

    HealthComponent->StartHealStaminaTimer();

    RollingAnimInProgress = false;
}

bool UFMBCharacterMovementComponent::CanRolling() const
{
    return !RollingAnimInProgress;
}
