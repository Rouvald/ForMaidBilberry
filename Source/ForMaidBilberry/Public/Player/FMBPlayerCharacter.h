// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Player/FMBBaseCharacter.h"
#include "FMBPlayerCharacter.generated.h"

class UFMBCharacterMovementComponent;
class USpringArmComponent;
class UCameraComponent;
class UFMBStaminaComponent;
class UFMBPlayerWeaponComponent;
class USphereComponent;
class UFMBItemInteractionComponent;

/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API AFMBPlayerCharacter : public AFMBBaseCharacter
{
    GENERATED_BODY()
public:
    AFMBPlayerCharacter(const FObjectInitializer& ObjInit);

    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual bool IsRunning() override;

protected:
    FName FPPCameraSocketName = "FPPCameraSocket";

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UFMBCharacterMovementComponent* CharacterMovementComponent{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* TPPCameraComponent{nullptr};

    /*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USphereComponent* TPPCameraCollisionComponent{nullptr};*/

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* FPPCameraComponent{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UFMBPlayerWeaponComponent* WeaponComponent{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UFMBStaminaComponent* StaminaComponent{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UFMBItemInteractionComponent* ItemInteractionComponent{nullptr};

    virtual void BeginPlay() override;
    virtual void OnDeath() override;

private:
    FTimerHandle ReportNoiseTimerHandle;

    bool bWantToRun{false};
    bool bIsMovingForward{false};

    void MoveForward(float Amount);
    void MoveRight(float Amount);

    virtual void Jump() override;

    void OnStartRunning();
    void OnStopRunning();

    void MakeReportNoise();
};
