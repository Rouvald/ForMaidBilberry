// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "FMBBaseWeapon.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "FMBCoreTypes.h"
#include "FMBBaseCharacter.generated.h"

class UFMBHealthComponent;
class UFMBWeaponComponent;
class UFMBCharacterMovementComponent;
class USoundCue;

UCLASS()
class FORMAIDBILBERRY_API AFMBBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AFMBBaseCharacter(const FObjectInitializer& ObjInit);

    virtual void Tick(float DeltaTime) override;

    // UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual bool IsRunning() const;

    // UFUNCTION(BlueprintCallable, Category="Movement")
    // float GetMovementDirection() const;

    // void SetTeamSkeletalMesh(USkeletalMesh* TeamSkeletalMesh) const;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Backpack")
    UStaticMeshComponent* Backpack{nullptr};

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName BackpackSocketName{TEXT("Backpack")};

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UFMBHealthComponent* HealthComponent{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UFMBWeaponComponent* WeaponComponent{nullptr};

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float LifeSpanOnDeath{5.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedVelocityZ{900.f, 1200.0f};

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamage{10.f, 100.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* CharacterDeathSound{nullptr};

    virtual void BeginPlay() override;

    virtual void OnDeath();
    virtual void OnHealthChange(float Health, float HealthDelta);

private:
    UFUNCTION()
    void OnGroundLanded(const FHitResult& HitResult);
};
