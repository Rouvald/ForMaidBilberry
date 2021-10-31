// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "FMBCoreTypes.h"
#include "GameFramework/Actor.h"
#include "FMBBaseWeapon.generated.h"

class UStaticMeshComponent;
class UFMBWeaponFXComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class FORMAIDBILBERRY_API AFMBBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    AFMBBaseWeapon();

    virtual void FastMeleeAttack();
    virtual void StrongMeleeAttack();

    //virtual void StrongMeleeAttack();

    void StopDrawTrace();

    FWeaponUIData GetWeaponUIData() const { return WeaponUIData; }

    UNiagaraComponent* GetSwordTrailFXComponent() const { return SwordTrailFXComponent; }

protected:
    UPROPERTY()
    USceneComponent* DefaultRootComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    UStaticMeshComponent* WeaponMesh;

    //UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon")
    //USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    UFMBWeaponFXComponent* WeaponFXComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName StartBladeTraceSocketName = "StartBladeTraceSocket";

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName EndBladeTraceSocketName = "EndBladeTraceSocket";

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName SwordTrailSocketName = "SwordTrailSocket";

    UPROPERTY()
    float DamageAmount;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    float FastAttackDamage = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
    float StrongAttackDamage = 30.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
    FWeaponUIData WeaponUIData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
    UNiagaraSystem* SwordTrailFX;

    FTimerHandle DrawTraceTimerHandle;

    UPROPERTY()
    TArray<ACharacter*> HitActors;

    UPROPERTY()
    UNiagaraComponent* SwordTrailFXComponent;

    virtual void BeginPlay() override;

    APlayerController* GetPlayerController() const;

    void TraceLogic();
    void DrawTrace();

    FVector FindBladeSocketLocation(FName BladeTraceSocketName) const;
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;

    void SortEqualCharacter(FHitResult& HitResult);
    void NewDamagedActor(FHitResult& HitResult);
    void MakeDamage(FHitResult& HitResult);

    void StartDrawTrace();

    UNiagaraComponent* SpawnSwordTrailFX() const;
};
