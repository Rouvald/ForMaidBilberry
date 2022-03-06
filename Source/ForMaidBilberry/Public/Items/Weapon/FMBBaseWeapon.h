// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "FMBCoreTypes.h"
#include "GameFramework/Actor.h"
#include "Items/FMBBaseItem.h"
#include "FMBBaseWeapon.generated.h"

class AFMBBaseItem;
class UStaticMeshComponent;
class UFMBWeaponFXComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class USoundCue;

UCLASS()
class FORMAIDBILBERRY_API AFMBBaseWeapon : public AFMBBaseItem
{
    GENERATED_BODY()

public:
    AFMBBaseWeapon();

    virtual void MeleeAttack(const EChooseAttack ChooseAttack);

    // UFUNCTION(BlueprintCallable)
    // FWeaponAnimationData GetWeaponAnimationData() const {return WeaponAnimationData;}

    // UFUNCTION(BlueprintCallable)
    FORCEINLINE const EWeaponType& GetWeaponType() const { return WeaponType; }

    void StopDrawTrace();

    FORCEINLINE const FWeaponUIData& GetWeaponUIData() const { return WeaponUIData; }
    FORCEINLINE UNiagaraComponent* GetSwordTrailFXComponent() const { return SwordTrailFXComponent; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
    UFMBWeaponFXComponent* WeaponFXComponent{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
    UNiagaraComponent* SwordTrailFXComponent{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    EWeaponType WeaponType{EWeaponType::EWT_RedSword};

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName StartBladeTraceSocketName{"StartBladeTraceSocket"};

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName EndBladeTraceSocketName{"EndBladeTraceSocket"};

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName SwordTrailSocketName{"SwordTrailSocket"};

    UPROPERTY()
    float TraceRadius{10.0f};

    UPROPERTY()
    float DamageAmount{0};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float FastAttackDamage{10.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float StrongAttackDamage{30.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData WeaponUIData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* SwordSlashSound{nullptr};

    FTimerHandle DrawTraceTimerHandle;

    UPROPERTY()
    TArray<AActor*> HitActors;

    TMap<EChooseAttack, float> ChooseDamageAmount;

    virtual void BeginPlay() override;

    void DrawTrace();

    FVector FindBladeSocketLocation(const FName BladeTraceSocketName) const;
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;

    void SortEqualCharacter(const FHitResult& HitResult);
    void NewDamagedActor(const FHitResult& HitResult);
    void MakeDamage(const FHitResult& HitResult);

    void StartDrawTrace();

    void SpawnSwordSlashSound() const;
};
