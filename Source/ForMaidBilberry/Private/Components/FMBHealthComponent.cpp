// For Maid Bilberry Game. All Rights Recerved

#include "Components/FMBHealthComponent.h"
#include "FMBBaseCharacter.h"
#include "Components/FMBCharacterMovementComponent.h"
#include "Components/FMBWeaponComponent.h"
#include "FMBUtils.h"
#include "FMBCoreTypes.h"
#include "FMBGameModeBase.h"
#include "AIController.h"

DECLARE_LOG_CATEGORY_CLASS(HealthLog, All, All);

UFMBHealthComponent::UFMBHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UFMBHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0);
    SetHealth(MaxHealth);

    const auto Controller = GetOwner();
    if (Controller)
    {
        Controller->OnTakeAnyDamage.AddDynamic(this, &UFMBHealthComponent::OnTakeAnyDamage);
    }
}

void UFMBHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead() || !GetWorld()) return;

    if (InstigatedBy)
    {
        const auto DamagedPawn = Cast<APawn>(GetOwner());
        if (!DamagedPawn) return;

        if (AreBothBots(DamagedPawn->GetController(), InstigatedBy)) return;
    }

    // TODO: Hit anim don't loop, when periodical damage  // NEED FIX or not XD
    /*if (CheckAllAnimInProgress())
    {
        PlayHitAnimation();
    }*/

    SetHealth(Health - Damage);
    GetWorld()->GetTimerManager().ClearTimer(HealTimeHandle);

    // UE_LOG(HealthLog, Display, TEXT("Health: %f"), GetHealth());

    if (IsDead())
    {
        Killed(InstigatedBy);
        OnDeath.Broadcast();
    }
    else if (AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimeHandle, this, &UFMBHealthComponent::AutoHealing, HealUpdateTime, true, HealDelay);
    }
    PlayCameraShake();
}

void UFMBHealthComponent::AutoHealing()
{
    SetHealth(Health + HealModifier);

    if (IsHealthFull() && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimeHandle);
    }
}

void UFMBHealthComponent::SetHealth(float NewHealth)
{
    const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    const auto HealthDelta = NextHealth - Health;

    Health = NextHealth;
    OnHealthChange.Broadcast(Health, HealthDelta);
}

bool UFMBHealthComponent::TryToAddHealth(float HealthAmount)
{
    if (IsDead() || IsHealthFull()) return false;

    SetHealth(Health + HealthAmount);
    return true;
}

bool UFMBHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

bool UFMBHealthComponent::CheckAllAnimInProgress() const
{
    const auto MovementComponent = FMBUtils::GetFMBPlayerComponent<UFMBCharacterMovementComponent>(GetOwner());
    if (!MovementComponent || !(MovementComponent->CanRolling())) return false;

    const auto WeaponComponent = FMBUtils::GetFMBPlayerComponent<UFMBWeaponComponent>(GetOwner());
    if (!WeaponComponent || !(WeaponComponent->CanAttack()) || !(WeaponComponent->CanEquip())) return false;
    return true;
}

void UFMBHealthComponent::PlayHitAnimation() const
{
    if (const auto Character = Cast<AFMBBaseCharacter>(GetOwner()))
    {
        const auto WeaponComponent = FMBUtils::GetFMBPlayerComponent<UFMBWeaponComponent>(Character);
        if (WeaponComponent || WeaponComponent->GetCurrentWeapon())
        {
            Character->PlayAnimMontage(WeaponComponent->GetCurrentWeaponAnimationsData().GetHit);
        }
        //==================================================
        // UE_LOG(HealthLog, Display, TEXT("GetHit Animation play"),);
        //==================================================
    }
}

void UFMBHealthComponent::PlayCameraShake() const
{
    if (IsDead()) return;
    const auto Player = Cast<APawn>(GetOwner());
    if (!Player) return;

    const auto PlayerController = Player->GetController<APlayerController>();
    if (!PlayerController || !PlayerController->PlayerCameraManager) return;

    PlayerController->PlayerCameraManager->StartCameraShake(CameraShake);
}

void UFMBHealthComponent::Killed(AController* KillerController)
{
    if (!GetWorld()) return;

    const auto GameMode = Cast<AFMBGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode) return;

    const auto Player = Cast<APawn>(GetOwner());
    const auto VictimController = Player ? Player->GetController() : nullptr;

    Cast<APlayerController>(KillerController) ? GameMode->PlayerKiller(KillerController, VictimController)
                                              : GameMode->BotKiller(VictimController);
}

bool UFMBHealthComponent::AreBothBots(AController* Controller1, AController* Controller2) const
{
    if (!Controller1 || !Controller2 || Controller1 == Controller2) return false;
    return !Controller1->IsPlayerController() && !Controller2->IsPlayerController();
}
