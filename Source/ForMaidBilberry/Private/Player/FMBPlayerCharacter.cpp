// For Maid Bilberry Game. All Rights Reserved

#include "Player/FMBPlayerCharacter.h"

#include "FMBItemInteractionComponent.h"
#include "FMBPlayerWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Perception/AISense_Hearing.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/FMBCharacterMovementComponent.h"
#include "Components/FMBStaminaComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFMBPlayerCharacter, All, All)

// constexpr static float DeltaSpringArm = 10.0f;

AFMBPlayerCharacter::AFMBPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->SetCapsuleRadius(55.0f);
    GetMesh()->SetRelativeLocation(FVector{-16.0f, 0.0f, -87.0f});

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    /*SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->SocketOffset = FVector(0.0f, 0.0f, 50.0f);
    SpringArmComponent->TargetArmLength = DefaultTargetArmLength;
    SpringArmComponent->bUsePawnControlRotation = true;

    TPPCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TPPCameraComponent"));
    TPPCameraComponent->SetupAttachment(SpringArmComponent);
    TPPCameraComponent->bUsePawnControlRotation = false;
    TPPCameraComponent->SetAutoActivate(false);

    TPPCameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("TPPCameraCollisionComponent");
    TPPCameraCollisionComponent->SetupAttachment(TPPCameraComponent);
    TPPCameraCollisionComponent->SetSphereRadius(10.0f);
    TPPCameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    TPPCameraCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);*/

    FPPCameraComponent = CreateDefaultSubobject<UCameraComponent>("FPPCameraComponent");
    FPPCameraComponent->SetupAttachment(GetMesh(), FPPCameraSocketName);
    FPPCameraComponent->SetRelativeLocation(FVector(2.0f, 37.0f, 0.0f));
    FPPCameraComponent->SetRelativeRotation(FRotator(90.0f, 180.0f, -90.0f));
    FPPCameraComponent->bUsePawnControlRotation = true;
    FPPCameraComponent->SetFieldOfView(100.0f);
    FPPCameraComponent->SetAutoActivate(true);

    StaminaComponent = CreateDefaultSubobject<UFMBStaminaComponent>(TEXT("StaminaComponent"));
    WeaponComponent = CreateDefaultSubobject<UFMBPlayerWeaponComponent>(TEXT("WeaponComponent"));
    ItemInteractionComponent = CreateDefaultSubobject<UFMBItemInteractionComponent>(TEXT("UFMBItemInteractionComponent"));
}

void AFMBPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    checkf(StaminaComponent, TEXT("StaminaComponent == nullptr"));
    checkf(ItemInteractionComponent, TEXT("ItemInteractionComponent == nullptr"));
    /*checkf(SpringArmComponent, TEXT("SpringArmComponent == nullptr"));
    checkf(TPPCameraCollisionComponent, TEXT("TPPCameraCollisionComponent == nullptr"));*/
    GetWorldTimerManager().SetTimer(ReportNoiseTimerHandle, this, &AFMBPlayerCharacter::MakeReportNoise, 0.1f, true);

    /*TPPCameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFMBPlayerCharacter::OnCameraCollisionBeginOverlap);
    TPPCameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AFMBPlayerCharacter::OnCameraCollisionEndOverlap);*/
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

void AFMBPlayerCharacter::CheckCameraOverlap() const
{
    // if(FPPCameraComponent->IsActive()) return;
    const bool HideMesh{TPPCameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent())};
    GetMesh()->SetOwnerNoSee(HideMesh);

    TArray<USceneComponent*> MeshChildren;
    GetMesh()->GetChildrenComponents(true, MeshChildren);
    for (const auto MeshChild : MeshChildren)
    {
        const auto MeshChildPrimitive = Cast<UPrimitiveComponent>(MeshChild);
        if (MeshChildPrimitive)
        {
            MeshChildPrimitive->SetOwnerNoSee(HideMesh);
        }
    }
}*/

void AFMBPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    checkf(PlayerInputComponent, TEXT("PlayerInputComponent == nullptr"));
    checkf(WeaponComponent, TEXT("WeaponComponent == nullptr"));
    CharacterMovementComponent = Cast<UFMBCharacterMovementComponent>(GetCharacterMovement());
    checkf(CharacterMovementComponent, TEXT("CharacterMovementComponent == nullptr"));

    PlayerInputComponent->BindAxis("MoveForward", this, &AFMBPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AFMBPlayerCharacter::MoveRight);

    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &APawn::AddControllerYawInput);
    // PlayerInputComponent->BindAxis("LookUpRate", this, &AFMBPlayerCharacter::LookUpAtRate);
    // PlayerInputComponent->BindAxis("TurnAroundRate", this, &AFMBPlayerCharacter::TurnAroundAtRate);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFMBPlayerCharacter::Jump);

    // PlayerInputComponent->BindAction("SwitchCamera", IE_Pressed, this, &AFMBPlayerCharacter::SwitchCamera);

    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AFMBPlayerCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &AFMBPlayerCharacter::OnStopRunning);

    PlayerInputComponent->BindAction("FastAttack", IE_Pressed, WeaponComponent, &UFMBBaseWeaponComponent::FastMeleeAttack);
    PlayerInputComponent->BindAction("StrongAttack", IE_Pressed, WeaponComponent, &UFMBBaseWeaponComponent::StrongMeleeAttack);

    // PlayerInputComponent->BindAction("UsePickUp", IE_Pressed, WeaponComponent, &UFMBBaseWeaponComponent::UsePickUp);

    // PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UFMBBaseWeaponComponent::NextWeapon);

    // PlayerInputComponent->BindAction("Rolling", IE_Pressed, CharacterMovementComponent, &UFMBCharacterMovementComponent::Rolling);

    // PlayerInputComponent->BindAction("Block", IE_Pressed, WeaponComponent, &UFMBPlayerWeaponComponent::OnStartBlock);
    // PlayerInputComponent->BindAction("Block", IE_Released, WeaponComponent, &UFMBPlayerWeaponComponent::OnStopBlock);

    PlayerInputComponent->BindAction(
        "Interact", IE_Pressed, ItemInteractionComponent, &UFMBItemInteractionComponent::TakeItemButtonPressed);
    // PlayerInputComponent->BindAction(
    //    "Interact", IE_Released, ItemInteractionComponent, &UFMBItemInteractionComponent::TakeItemButtonReleased);

    // PlayerInputComponent->BindAction("Throw", IE_Pressed, ItemInteractionComponent, &UFMBItemInteractionComponent::Throw);

    PlayerInputComponent->BindAction("UsePickUp", IE_Pressed, WeaponComponent, &UFMBPlayerWeaponComponent::UsePickUp);

    /*
     PlayerInputComponent->BindAction("ReturnSpringArm", IE_Pressed, this, &AFMBPlayerCharacter::ReturnDefaultSpringArm);

    DECLARE_DELEGATE_OneParam(FOnSpringArmChangeSignature, bool);
    PlayerInputComponent->BindAction<FOnSpringArmChangeSignature>("DistanceSpringArm", IE_Repeat, this,
        &AFMBPlayerCharacter::ChangeSpringArm, true);
    PlayerInputComponent->BindAction<FOnSpringArmChangeSignature>("CloserSpringArm", IE_Repeat, this,
        &AFMBPlayerCharacter::ChangeSpringArm, false);
        */
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
}*/

/*void AFMBPlayerCharacter::SwitchCamera()
{
    if (FPPCameraComponent->IsActive())
    {
        FPPCameraComponent->SetActive(false);
        TPPCameraComponent->SetActive(true);
        TPPCameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    }
    else
    {
        FPPCameraComponent->SetActive(true);
        TPPCameraComponent->SetActive(false);
        TPPCameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    }
}*/

void AFMBPlayerCharacter::MoveForward(float Amount)
{
    bIsMovingForward = Amount > 0.0f;
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
    if (!WeaponComponent || !(WeaponComponent->CanAttack())) return;

    Super::Jump();
}

void AFMBPlayerCharacter::OnStartRunning()
{
    if (!CharacterMovementComponent || !(CharacterMovementComponent->CanRolling()) || CharacterMovementComponent->IsFalling()) return;

    if (GetVelocity().IsNearlyZero()) return;

    if (!StaminaComponent) return;
    StaminaComponent->StartStaminaRunning();

    bWantToRun = true;
}

void AFMBPlayerCharacter::OnStopRunning()
{
    if (!StaminaComponent) return;
    StaminaComponent->StopStaminaRunning();

    bWantToRun = false;
}

bool AFMBPlayerCharacter::IsRunning()
{
    /*const auto bIsRunning {};
    if(!bIsRunning)
    {
        OnStopRunning();
        UE_LOG(LogFMBPlayerCharacter, Display, TEXT("!bIsRunning"));
    }*/
    return bWantToRun && bIsMovingForward && !(FMath::IsNearlyZero(StaminaComponent->GetStamina())) && !GetVelocity().IsZero();
}

void AFMBPlayerCharacter::OnDeath()
{
    Super::OnDeath();
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
    WeaponComponent->StopDrawTrace();
    StaminaComponent->StopHealStamina();
    StaminaComponent->StopStaminaRunning();
    ItemInteractionComponent->StopItemInfoVisibility();
}

void AFMBPlayerCharacter::MakeReportNoise()
{
    UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.0f, this, 1000.0f);
}

/*void AFMBPlayerCharacter::ReturnDefaultSpringArm()
{
    const float CurrentSpringArmLength{SpringArmComponent->TargetArmLength};
    InterpSpringArm(CurrentSpringArmLength, DefaultTargetArmLength);
}*/

/*
 *void AFMBPlayerCharacter::DistanceSpringArm()
{
    const float NewSpringArmLength {SpringArmComponent->TargetArmLength + DeltaSpringArm};
    InterpSpringArm(SpringArmComponent->TargetArmLength, NewSpringArmLength);
}
*/

/*void AFMBPlayerCharacter::ChangeSpringArm(bool bIsDistance)
{
    float NewSpringArmLength{
        bIsDistance ? SpringArmComponent->TargetArmLength + DeltaSpringArm : SpringArmComponent->TargetArmLength - DeltaSpringArm};
    if (NewSpringArmLength < TargetArmLengthVector2D.X || NewSpringArmLength > TargetArmLengthVector2D.Y) return;

    NewSpringArmLength = FMath::Clamp<float>(NewSpringArmLength, TargetArmLengthVector2D.X, TargetArmLengthVector2D.Y);
    InterpSpringArm(SpringArmComponent->TargetArmLength, NewSpringArmLength);
}

void AFMBPlayerCharacter::InterpSpringArm(const float Start, const float End) const
{
    if (!GetWorld()) return;
    float InterpArm{Start};

    InterpArm = FMath::FInterpTo(InterpArm, End, GetWorld()->GetDeltaSeconds(), 120.0f);
    UE_LOG(LogFMBPlayerCharacter, Display, TEXT("%f"), InterpArm);

    SpringArmComponent->TargetArmLength = InterpArm;
}*/
