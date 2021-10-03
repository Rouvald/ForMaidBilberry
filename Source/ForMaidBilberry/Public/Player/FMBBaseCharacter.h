// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "FMBBaseWeapon.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "FMBBaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UFMBHealthComponent;
class UTextRenderComponent;
class AFMBBaseWeapon;

UCLASS()
class FORMAIDBILBERRY_API AFMBBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AFMBBaseCharacter(const FObjectInitializer& ObjInit);

protected:
    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    //float BaseTurnAroundRate = 45.0f;

    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    //float BaseLookUpRate = 45.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UCameraComponent* TPPCameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UCameraComponent* FPPCameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UFMBHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UTextRenderComponent* HealthTextComponent;

    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category="Damage")
    float LifeSpanOnDeath = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category="Damage")
    FVector2D LandedVelocityZ = {900.f, 1200.0f};

    UPROPERTY(EditDefaultsOnly, Category="Damage")
    FVector2D LandedDamage = {10.f, 100.0f};

    UPROPERTY(EditDefaultsOnly, Category="Weapon")
    TSubclassOf<AFMBBaseWeapon> WeaponClass;

    virtual void BeginPlay() override;

public:
    FName FPPCameraSocketName = "FPPCameraSocket";

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category="Movement")
    bool IsRunning() const;

    //UFUNCTION(BlueprintCallable, Category="Movement")
    //float GetMovementDirection() const;

private:
    bool WantToRun = false;
    //bool IsMovingForward = false;

    void SwitchCamera();

    void MoveForward(float Amount);
    void MoveRight(float Amount);

    void LimitViewPitchRotation ();
    //void TurnAroundAtRate(float Rate);
    //void LookUpAtRate(float Rate);

    void OnStartRunning();
    void OnStopRunning();

    void OnDeath();
    void OnHealthChange(float Health);

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hitresult);

    void SpawnWeapon();
};
