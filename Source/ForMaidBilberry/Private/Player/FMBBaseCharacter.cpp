// For Maid Bilberry Game. All Rights Recerved


#include "Player/FMBBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/FMBCharacterMovementComponent.h"
#include "Components/FMBHealthComponent.h"
#include "Components/FMBWeaponComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"
#include "Weapon/FMBBaseWeapon.h"

DECLARE_LOG_CATEGORY_CLASS(BaseCharacterLog, All, All);

AFMBBaseCharacter::AFMBBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UFMBCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    // Camara rotate around Character
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    //

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->SocketOffset = FVector(0.0f, 0.0f, 50.0f);
    SpringArmComponent->TargetArmLength = 500.0f;
    SpringArmComponent->bUsePawnControlRotation = true;

    TPPCameraComponent = CreateDefaultSubobject<UCameraComponent>("TPPCameraComponent");
    TPPCameraComponent->SetupAttachment(SpringArmComponent);
    TPPCameraComponent->bUsePawnControlRotation = false;

    /*
    FPPCameraComponent = CreateDefaultSubobject<UCameraComponent>("FPPCameraComponent");
    FPPCameraComponent->SetupAttachment(GetMesh(), FPPCameraSocketName);
    FPPCameraComponent->SetRelativeLocation(FVector(-3.0f, 30.0f, 1.0f));
    FPPCameraComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 90.0f));
    FPPCameraComponent->bUsePawnControlRotation = true;
    */

    HealthComponent = CreateDefaultSubobject<UFMBHealthComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->SetOwnerNoSee(true);

    WeaponComponent = CreateDefaultSubobject<UFMBWeaponComponent>("WeaponComponent");
}

void AFMBBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());

    OnHealthChange(HealthComponent->GetHealth());
    HealthComponent->OnDeath.AddUObject(this, &AFMBBaseCharacter::OnDeath);
    HealthComponent->OnHealthChange.AddUObject(this, &AFMBBaseCharacter::OnHealthChange);

    LandedDelegate.AddDynamic(this, &AFMBBaseCharacter::OnGroundLanded);

    SetStamina(MaxStamina);
}

void AFMBBaseCharacter::OnHealthChange(float Health)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%0.0f"), Health)));
}

void AFMBBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AFMBBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    check(WeaponComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AFMBBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AFMBBaseCharacter::MoveRight);

    PlayerInputComponent->BindAxis("LookUp", this, &AFMBBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &AFMBBaseCharacter::AddControllerYawInput);

    //PlayerInputComponent->BindAxis("LookUpRate", this, &AFMBBaseCharacter::LookUpAtRate);
    //PlayerInputComponent->BindAxis("TurnAroundRate", this, &AFMBBaseCharacter::TurnAroundAtRate);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFMBBaseCharacter::Jump);

    //PlayerInputComponent->BindAction("SwitchCamera", EInputEvent::IE_Pressed, this, &AFMBBaseCharacter::SwitchCamera);

    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AFMBBaseCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &AFMBBaseCharacter::OnStopRunning);

    PlayerInputComponent->BindAction("FastMeleeAttack", IE_Pressed, WeaponComponent, &UFMBWeaponComponent::FastMeleeAttack);
    PlayerInputComponent->BindAction("StrongMeleeAttack", IE_Pressed, WeaponComponent, &UFMBWeaponComponent::StrongMeleeAttack);
}

/*void AFMBBaseCharacter::LimitViewPitchRotation ()//
{
    const auto PlayerController = Cast<APlayerController>(Controller);
    if (PlayerController && PlayerController->PlayerCameraManager)
    {
        PlayerController->PlayerCameraManager->ViewPitchMax = 50.0f;
        PlayerController->PlayerCameraManager->ViewPitchMin = 50.0f;
    }
}

void AFMBBaseCharacter::SwitchCamera()//
{
    
}*/
/*void AFMBBaseCharacter::TurnAroundAtRate(float Rate)
{
    AddControllerYawInput(Rate * BaseTurnAroundRate * GetWorld()->GetDeltaSeconds());
}

void AFMBBaseCharacter::LookUpAtRate(float Rate)
{
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}*/

void AFMBBaseCharacter::MoveForward(float Amount)
{
    //IsMovingForward = Amount > 0.0f;
    if (Amount == 0.0f) return;

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator RotationYaw(0.0f, Rotation.Yaw, 0.0f);

    const FVector Direction = FRotationMatrix(RotationYaw).GetUnitAxis(EAxis::X);

    AddMovementInput(Direction, Amount);
}

void AFMBBaseCharacter::MoveRight(float Amount)
{
    if (Amount == 0.0f) return;

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator RotationYaw(0.0f, Rotation.Yaw, 0.0f);

    const FVector Direction = FRotationMatrix(RotationYaw).GetUnitAxis(EAxis::Y);

    AddMovementInput(Direction, Amount);
}

void AFMBBaseCharacter::OnStartRunning()
{
    if (GetVelocity().IsNearlyZero()) return;
    if (GetWorld()->GetTimerManager().IsTimerActive(StaminaAutoHealTimerHandle))
    {
        GetWorld()->GetTimerManager().ClearTimer(StaminaAutoHealTimerHandle);
    }

    GetWorld()->GetTimerManager().SetTimer
            (
            StaminaRunningTimerHandle,
            this,
            &AFMBBaseCharacter::DecreaseRunningStamina,
            StaminaUpdateTime,
            true
            );
    WantToRun = true;
}

void AFMBBaseCharacter::OnStopRunning()
{
    GetWorld()->GetTimerManager().ClearTimer(StaminaRunningTimerHandle);
    WantToRun = false;
    GetWorld()->GetTimerManager().SetTimer(StaminaAutoHealTimerHandle, this, &AFMBBaseCharacter::AutoHealStamina, StaminaUpdateTime, true,
        StaminaAutoHealDelay);
}

bool AFMBBaseCharacter::IsRunning() const
{
    return WantToRun && GetStamina() > 0.0f && !GetVelocity().IsZero();
}

/*float AFMBBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero()) return 0.0f;
    const auto VelosityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelosityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelosityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}*/

void AFMBBaseCharacter::OnDeath()
{
    UE_LOG(BaseCharacterLog, Display, TEXT("Player %s is dead"), *GetName());

    PlayAnimMontage(DeathAnimMontage);

    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(LifeSpanOnDeath);

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void AFMBBaseCharacter::OnGroundLanded(const FHitResult& Hitresult)
{
    const auto VelocityZ = GetVelocity().Z * (-1);
    UE_LOG(BaseCharacterLog, Display, TEXT("VelocityZ %f"), VelocityZ);

    if (VelocityZ < LandedVelocityZ.X) return;

    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedVelocityZ, LandedDamage, VelocityZ);
    UE_LOG(BaseCharacterLog, Display, TEXT("FinalDamage %f"), FinalDamage);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

void AFMBBaseCharacter::SetStamina(float NewStamina)
{
    Stamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);
    OnStaminaChange.Broadcast(Stamina);
}

void AFMBBaseCharacter::DecreaseRunningStamina()
{
    if (!GetWorld()) return;
    if (GetStamina() <= 0.0f)
    {
        GetWorld()->GetTimerManager().ClearTimer(StaminaRunningTimerHandle);
    }
    SetStamina(Stamina - StaminaModifier);
    UE_LOG(BaseCharacterLog, Display, TEXT("Stamina change: %0.0f"), GetStamina());
}

void AFMBBaseCharacter::AutoHealStamina()
{
    SetStamina(Stamina + StaminaModifier);
    UE_LOG(BaseCharacterLog, Display, TEXT("Stamina change: %0.0f"), GetStamina());

    if (FMath::IsNearlyEqual(Stamina, MaxStamina) && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(StaminaAutoHealTimerHandle);
    }
}