// For Maid Bilberry Game. All Rights Recerved


#include "Player/FMBBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/FMBCharacterMovementComponent.h"

AFMBBaseCharacter::AFMBBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UFMBCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    TPPCameraComponent = CreateDefaultSubobject<UCameraComponent>("TPPCameraComponent");
    TPPCameraComponent->SetupAttachment(SpringArmComponent);

    //FPPCameraComponent = CreateDefaultSubobject<UCameraComponent>("FPPCameraComponent");
    //FPPCameraComponent->SetupAttachment(GetMesh(), FPPCameraSocketName);
    //FPPCameraComponent->SetAutoActivate(false);
}

void AFMBBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AFMBBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AFMBBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AFMBBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AFMBBaseCharacter::MoveRight);

    PlayerInputComponent->BindAxis("LookUp", this, &AFMBBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &AFMBBaseCharacter::AddControllerYawInput);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFMBBaseCharacter::Jump);

    //PlayerInputComponent->BindAction("SwitchCamera", EInputEvent::IE_Pressed, this, &AFMBBaseCharacter::SwitchCamera);

    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AFMBBaseCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &AFMBBaseCharacter::OnStopRunning);
}

/*void AFMBBaseCharacter::SwitchCamera()
{
    if (TPPCameraComponent->bAutoActivate)
    {
        FPPCameraComponent->SetAutoActivate(true);
        TPPCameraComponent->SetAutoActivate(false);
    }
    else if (FPPCameraComponent->bAutoActivate)
    {
        TPPCameraComponent->SetAutoActivate(true);
        FPPCameraComponent->SetAutoActivate(false);
    }
}*/

void AFMBBaseCharacter::MoveForward(float Amount)
{
    IsMovingForward = Amount > 0.0f;
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorForwardVector(), Amount);
}

void AFMBBaseCharacter::MoveRight(float Amount)
{
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorRightVector(), Amount);
}

void AFMBBaseCharacter::OnStartRunning()
{
    WantToRun = true;
}

void AFMBBaseCharacter::OnStopRunning()
{
    WantToRun = false;
}

bool AFMBBaseCharacter::IsRunning() const
{
    return WantToRun && IsMovingForward && !GetVelocity().IsZero();
}

float AFMBBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero()) return 0.0f;
    const auto VelosityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelosityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelosityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}
