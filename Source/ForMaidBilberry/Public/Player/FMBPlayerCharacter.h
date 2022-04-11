// For Maid Bilberry Game. All Rights Recerved

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
    virtual bool IsRunning() const override;

    // FORCEINLINE bool GetIsFPP() const { return FPPCameraComponent->IsActive(); }

protected:
    FName FPPCameraSocketName = "FPPCameraSocket";

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UFMBCharacterMovementComponent* CharacterMovementComponent{nullptr};

    /*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* TPPCameraComponent{nullptr};*/

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
    /*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
    float DefaultTurnAroundRate{45.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
    float DefaultLookUpRate{45.0f};*/

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    FVector2D TargetArmLengthVector2D{300.0f, 800.0f};

    float DefaultTargetArmLength{500.0f};

    FTimerHandle ReportNoiseTimerHandle;

    bool WantToRun{false};

    void MoveForward(float Amount);
    void MoveRight(float Amount);

    virtual void Jump() override;

    /*void TurnAroundAtRate(float Rate);
    void LookUpAtRate(float Rate);*/

    void OnStartRunning();
    void OnStopRunning();

    /*UFUNCTION()
    void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnCameraCollisionEndOverlap(
        UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void CheckCameraOverlap() const;*/

    void MakeReportNoise();
    // void SwitchCamera();

    // void ReturnDefaultSpringArm();
    // void ChangeSpringArm(bool bIsDistance);
    // void DistanceSpringArm();
    // void InterpSpringArm(const float Start, const float End) const;
};
