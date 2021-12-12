// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "FMBBaseWeapon.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "FMBCoreTypes.h"
#include "FMBBaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UFMBHealthComponent;
class UTextRenderComponent;
class UFMBWeaponComponent;

UCLASS()
class FORMAIDBILBERRY_API AFMBBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    //FName FPPCameraSocketName = "FPPCameraSocket";

    AFMBBaseCharacter(const FObjectInitializer& ObjInit);

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category="Movement")
    bool IsRunning() const;

    //UFUNCTION(BlueprintCallable, Category="Movement")
    //float GetMovementDirection() const;

    //void SetTeamSkeletalMesh(USkeletalMesh* TeamSkeletalMesh) const;

protected:
    UPROPERTY(EditDefaultsOnly, Category="Backpack")
    UStaticMeshComponent* Backpack;

    UPROPERTY(EditDefaultsOnly, Category="Weapon")
    FName BackpackSocketName = "Backpack";

    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    //float BaseTurnAroundRate = 45.0f;

    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    //float BaseLookUpRate = 45.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UCameraComponent* TPPCameraComponent;

    /*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USphereComponent* CameraCollisionComponent;*/

    //UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    //UCameraComponent* FPPCameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UFMBHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UTextRenderComponent* HealthTextComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    UFMBWeaponComponent* WeaponComponent;

    //UPROPERTY(EditDefaultsOnly, Category="Animation")
    //UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category="Damage")
    float LifeSpanOnDeath = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category="Damage")
    FVector2D LandedVelocityZ = {900.f, 1200.0f};

    UPROPERTY(EditDefaultsOnly, Category="Damage")
    FVector2D LandedDamage = {10.f, 100.0f};

    virtual void BeginPlay() override;

    virtual void OnDeath();

private:
    bool WantToRun = false;
    //bool IsMovingForward = false;

    //bool JumpAnimInProgress = false;

    //void SwitchCamera();

    void MoveForward(float Amount);
    void MoveRight(float Amount);

    virtual void Jump() override;
    //bool CanJump() const;

    //void LimitViewPitchRotation ();
    //void TurnAroundAtRate(float Rate);
    //void LookUpAtRate(float Rate);

    void OnStartRunning();
    void OnStopRunning();
    
    void OnHealthChange(float Health, float HealthDelta);

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hitresult);

    //void FastMeleeAttack();
    //void StrongMeleeAttack();

    //bool CheckAllAnimInProgress() const;

    //void CheckCameraOverlap();
};
