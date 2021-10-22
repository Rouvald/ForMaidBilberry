// For Maid Bilberry Game. All Rights Recerved


#include "Player/FMBBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
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
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->bUseControllerDesiredRotation = true;
    GetCharacterMovement()->bIgnoreBaseRotation = true;
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

    GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -86.0f), FRotator(0.0f, -90.0f, 0.0f));

    HealthComponent = CreateDefaultSubobject<UFMBHealthComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->SetOwnerNoSee(true);

    WeaponComponent = CreateDefaultSubobject<UFMBWeaponComponent>("WeaponComponent");

    Backpack = CreateDefaultSubobject<UStaticMeshComponent>("Backpack");
    Backpack->SetupAttachment(GetMesh(), BackpackSocketName);
    Backpack->SetCollisionResponseToChannels(ECollisionResponse::ECR_Overlap);
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

    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UFMBWeaponComponent::NextWeapon);

    PlayerInputComponent->BindAction("Rolling", IE_Pressed, this, &AFMBBaseCharacter::Rolling);
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

void AFMBBaseCharacter::Jump()
{
    const auto MovementComponent = Cast<UFMBCharacterMovementComponent>(GetMovementComponent());
    if (!MovementComponent || !(MovementComponent->CanRolling())) return;

    if (!WeaponComponent || !(WeaponComponent->CanAttack())) return;

    //if (!SpendStamina(JumpStaminaSpend)) return;
    Super::Jump();
}

bool AFMBBaseCharacter::SpendStamina(int32 SpendStaminaValue) const
{
    if(!(HealthComponent->SpendStamina(SpendStaminaValue))) return false;
    return true;
}

void AFMBBaseCharacter::OnStartRunning()
{
    const auto MovementComponent = Cast<UFMBCharacterMovementComponent>(GetMovementComponent());
    if (!MovementComponent || !(MovementComponent->CanRolling()) || MovementComponent->IsFalling()) return;

    if (!WeaponComponent || !(WeaponComponent->CanAttack())) return;

    if (GetVelocity().IsNearlyZero()) return;

    if(!HealthComponent) return;
    HealthComponent->StartStaminaRunningTimer();

    WantToRun = true;
}

void AFMBBaseCharacter::OnStopRunning()
{
    if(!HealthComponent) return;
    HealthComponent->CheckAndStopStaminaRunningTimer();
    
    WantToRun = false;
    
    HealthComponent->StartHealStaminaTimer();
}

bool AFMBBaseCharacter::IsRunning() const
{
    return WantToRun && !(FMath::IsNearlyZero(HealthComponent->GetStamina())) && !GetVelocity().IsZero();
}

/*
void AFMBBaseCharacter::FastMeleeAttack()
{
    if (!CheckAllAnimInProgress()) return;
    if(GetCharacterMovement()->IsFalling()) return;

    if (!SpendStamina(FastAttackStaminaSpend)) return;

    WeaponComponent->FastMeleeAttack();
}

void AFMBBaseCharacter::StrongMeleeAttack()
{
    if (!CheckAllAnimInProgress()) return;
    if(GetCharacterMovement()->IsFalling()) return;

    if (!SpendStamina(StrongAttackStaminaSpend)) return;

    WeaponComponent->StrongMeleeAttack();
}
*/
/*
float AFMBBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero()) return 0.0f;
    const auto VelosityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelosityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelosityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}
*/

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
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    
    HealthComponent->CheckAndStopHealStaminaTimer();
    HealthComponent->CheckAndStopStaminaRunningTimer();
    
    WeaponComponent->StopDrawTrace();
    
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
}

void AFMBBaseCharacter::OnGroundLanded(const FHitResult& Hitresult)
{
    const auto VelocityZ = GetVelocity().Z * (-1);
    //UE_LOG(BaseCharacterLog, Display, TEXT("VelocityZ %f"), VelocityZ);

    if (VelocityZ < LandedVelocityZ.X) return;

    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedVelocityZ, LandedDamage, VelocityZ);
    UE_LOG(BaseCharacterLog, Display, TEXT("FinalDamage %f"), FinalDamage);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

void AFMBBaseCharacter::Rolling()
{
    const auto MovementComponent = FindComponentByClass<UFMBCharacterMovementComponent>();
    if (!MovementComponent || !(MovementComponent->CanRolling())) return;

    if (!WeaponComponent || !(WeaponComponent->CanEquip())) return;
    
    if (MovementComponent->IsFalling()) return;

    if(WeaponComponent->CanAttack())
    {
        if (GetVelocity().IsZero()) return;
    }

    if (!SpendStamina(RollingStaminaSpend)) return;
    
    WeaponComponent->StopDrawTrace();
    OnStopRunning();
    MovementComponent->Rolling();
}

/*bool AFMBBaseCharacter::CheckAllAnimInProgress() const
{
    const auto MovementComponent = FindComponentByClass<UFMBCharacterMovementComponent>();
    if (!MovementComponent || !(MovementComponent->CanRolling())) return false;

    if (!WeaponComponent || !(WeaponComponent->CanAttack()) || !(WeaponComponent->CanEquip())) return false;
    return true;
}*/
