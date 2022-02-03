// For Maid Bilberry Game. All Rights Recerved

#include "Player/FMBPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/FMBCharacterMovementComponent.h"
#include "Components/FMBWeaponComponent.h"
#include "Components/FMBStaminaComponent.h"
#include "Perception/AISense_Hearing.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBPlayerCharacter, All, All)

AFMBPlayerCharacter::AFMBPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    PrimaryActorTick.bCanEverTick = true;

    // Camara rotate around Character
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;
    //
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->SocketOffset = FVector(0.0f, 0.0f, 50.0f);
    SpringArmComponent->TargetArmLength = 500.0f;
    SpringArmComponent->bUsePawnControlRotation = true;

    TPPCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TPPCameraComponent"));
    TPPCameraComponent->SetupAttachment(SpringArmComponent);
    TPPCameraComponent->bUsePawnControlRotation = false;

    /*
    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
    CameraCollisionComponent->SetupAttachment(TPPCameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.0f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    CameraCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    */

    /*
    FPPCameraComponent = CreateDefaultSubobject<UCameraComponent>("FPPCameraComponent");
    FPPCameraComponent->SetupAttachment(GetMesh(), FPPCameraSocketName);
    FPPCameraComponent->SetRelativeLocation(FVector(-3.0f, 30.0f, 1.0f));
    FPPCameraComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 90.0f));
    FPPCameraComponent->bUsePawnControlRotation = true;
    */

    StaminaComponent = CreateDefaultSubobject<UFMBStaminaComponent>(TEXT("StaminaComponent"));
}

void AFMBPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    checkf(StaminaComponent, TEXT("StaminaComponent = nullptr"));
    // check(CameraCollisionComponent);
    GetWorldTimerManager().SetTimer(ReportNoiseTimerHandle, this, &AFMBPlayerCharacter::MakeReportNoise, 0.1f, true);

    // CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFMBPlayerCharacter::OnCameraCollisionBeginOverlap);
    // CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AFMBPlayerCharacter::OnCameraCollisionEndOverlap);
}

/*void AFMBPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    CheckCameraOverlap();
}

void AFMBPlayerCharacter::OnCameraCollisionEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    CheckCameraOverlap();
}

void AFMBPlayerCharacter::CheckCameraOverlap()
{
    const bool HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
    GetMesh()->SetOwnerNoSee(HideMesh);

    TArray<USceneComponent*> MeshChildren;
    GetMesh()->GetChildrenComponents(true, MeshChildren);
    for(const auto MeshChild: MeshChildren)
    {
        const auto MeshChildPrimitive = Cast<UPrimitiveComponent>(MeshChild);
        if(MeshChildPrimitive)
        {
            MeshChildPrimitive->SetOwnerNoSee(HideMesh);
        }
    }
}*/

void AFMBPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    checkf(PlayerInputComponent, TEXT("PlayerInputComponent = nullptr"));
    checkf(WeaponComponent, TEXT("WeaponComponent = nullptr"));
    CharacterMovementComponent = Cast<UFMBCharacterMovementComponent>(GetCharacterMovement());
    checkf(CharacterMovementComponent, TEXT("CharacterMovementComponent = nullptr"));

    PlayerInputComponent->BindAxis("MoveForward", this, &AFMBPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AFMBPlayerCharacter::MoveRight);

    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &APawn::AddControllerYawInput);
    // PlayerInputComponent->BindAxis("LookUpRate", this, &AFMBPlayerCharacter::LookUpAtRate);
    // PlayerInputComponent->BindAxis("TurnAroundRate", this, &AFMBPlayerCharacter::TurnAroundAtRate);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFMBPlayerCharacter::Jump);

    // PlayerInputComponent->BindAction("SwitchCamera", EInputEvent::IE_Pressed, this, &AFMBBaseCharacter::SwitchCamera);

    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AFMBPlayerCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &AFMBPlayerCharacter::OnStopRunning);

    PlayerInputComponent->BindAction("FastAttack", IE_Pressed, WeaponComponent, &UFMBWeaponComponent::FastMeleeAttack);
    PlayerInputComponent->BindAction("StrongAttack", IE_Pressed, WeaponComponent, &UFMBWeaponComponent::StrongMeleeAttack);

    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UFMBWeaponComponent::NextWeapon);

    PlayerInputComponent->BindAction("Rolling", IE_Pressed, CharacterMovementComponent, &UFMBCharacterMovementComponent::Rolling);
}

// Try to switch between 2 cameras
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

void AFMBPlayerCharacter::MoveForward(float Amount)
{
    // IsMovingForward = Amount > 0.0f;
    if (FMath::IsNearlyZero(Amount)) return;

    const FRotator Rotation{Controller->GetControlRotation()};
    const FRotator RotationYaw{0.0f, Rotation.Yaw, 0.0f};

    const FVector Direction{FRotationMatrix(RotationYaw).GetUnitAxis(EAxis::X)};

    AddMovementInput(Direction, Amount);
}

void AFMBPlayerCharacter::MoveRight(float Amount)
{
    if (FMath::IsNearlyZero(Amount)) return;

    const FRotator Rotation{Controller->GetControlRotation()};
    const FRotator RotationYaw{0.0f, Rotation.Yaw, 0.0f};

    const FVector Direction{FRotationMatrix(RotationYaw).GetUnitAxis(EAxis::Y)};

    AddMovementInput(Direction, Amount);
}

/*
void AFMBPlayerCharacter::TurnAroundAtRate(float Rate)
{
    if (!GetWorld()) return;
    AddControllerYawInput(Rate * DefaultTurnAroundRate * GetWorld()->GetDeltaSeconds());
}

void AFMBPlayerCharacter::LookUpAtRate(float Rate)
{
    if (!GetWorld()) return;
    AddControllerPitchInput(Rate * DefaultLookUpRate * GetWorld()->GetDeltaSeconds());
}
*/

void AFMBPlayerCharacter::Jump()
{
    if (!CharacterMovementComponent || !(CharacterMovementComponent->CanRolling())) return;

    if (!WeaponComponent || !(WeaponComponent->CanAttack())) return;

    Super::Jump();
}

void AFMBPlayerCharacter::OnStartRunning()
{
    if (!CharacterMovementComponent || !(CharacterMovementComponent->CanRolling()) || CharacterMovementComponent->IsFalling()) return;

    if (GetVelocity().IsNearlyZero()) return;

    if (!StaminaComponent) return;
    StaminaComponent->StartStaminaRunning();

    WantToRun = true;
}

void AFMBPlayerCharacter::OnStopRunning()
{
    if (!StaminaComponent) return;
    StaminaComponent->StopStaminaRunning();

    WantToRun = false;

    StaminaComponent->StartHealStamina();
}

bool AFMBPlayerCharacter::IsRunning() const
{
    return WantToRun && !(FMath::IsNearlyZero(StaminaComponent->GetStamina())) && !GetVelocity().IsZero();
}

void AFMBPlayerCharacter::OnDeath()
{
    Super::OnDeath();
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }

    StaminaComponent->StopHealStamina();
    StaminaComponent->StopStaminaRunning();
}

void AFMBPlayerCharacter::MakeReportNoise()
{
    UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.0f, this, 1000.0f);
}
