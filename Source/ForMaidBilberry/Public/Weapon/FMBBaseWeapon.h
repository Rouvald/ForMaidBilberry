// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FMBBaseWeapon.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;

UCLASS()
class FORMAIDBILBERRY_API AFMBBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    AFMBBaseWeapon();

    virtual void FastMeleeAttack();

    virtual void StrongMeleeAttack();

    void StopDrawTrace();

protected:
    //UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    //UCapsuleComponent* CapsuleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName StartBladeTraceSocketName = "StartBladeTraceSocket";

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName EndBladeTraceSocketName = "EndBladeTraceSocket";

    UPROPERTY()
    float DamageAmount;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    float FastAttackDamage = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    float StrongAttackDamage = 30.0f;

    FTimerHandle DrawTraceTimerHandle;
    
    UPROPERTY()
    TArray<ACharacter*> HitActors;

    virtual void BeginPlay() override;

    APlayerController* GetPlayerController() const;

    void TraceLogic();
    void DrawTrace();

    FVector FindBladeSocketLocation(FName BladeTraceSocketName) const;
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

    void SortEqualCharacter(FHitResult& HitResult);
    void NewDamagedActor(FHitResult& HitResult);
    void MakeDamage(FHitResult& HitResult);

    void StartDrawTrace();
};
