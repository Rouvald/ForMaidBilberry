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

    // virtual void SetItemInfoWidgetVisibility(bool bIsVisible) const override;

    // UFUNCTION(BlueprintCallable)
    // FWeaponAnimationData GetWeaponAnimationData() const {return WeaponAnimationData;}

    // UFUNCTION(BlueprintCallable)
    FORCEINLINE const EWeaponType& GetWeaponType() const { return WeaponType; }

    void StopDrawTrace();

    FORCEINLINE UNiagaraComponent* GetSwordTrailFXComponent() const { return SwordTrailFXComponent; }
    FORCEINLINE const FWeaponDamageData& GetWeaponDamageData() const { return WeaponDamageData; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
    UFMBWeaponFXComponent* WeaponFXComponent{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
    UNiagaraComponent* SwordTrailFXComponent{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    EWeaponType WeaponType{EWeaponType::EWT_SwordShield};

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
    FWeaponDamageData WeaponDamageData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* SwordSlashSound{nullptr};

    FTimerHandle DrawTraceTimerHandle;

    UPROPERTY()
    TArray<AActor*> HitActors;

    TMap<EChooseAttack, float> ChooseDamageAmount;

    virtual void BeginPlay() override;

    virtual void SetItemInfo() const override;

    void DrawTrace();

    FVector FindBladeSocketLocation(const FName BladeTraceSocketName) const;
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;

    void SortEqualCharacter(const FHitResult& HitResult);
    void NewDamagedActor(const FHitResult& HitResult);
    void MakeDamage(const FHitResult& HitResult);

    void StartDrawTrace();

    // virtual float UpdateItemInfoProperties() const override;

    void SpawnSwordSlashSound() const;
};
