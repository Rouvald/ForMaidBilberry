// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapon/FMBBaseWeapon.h"
#include "FMBBaseMeleeWeapon.generated.h"

/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API AFMBBaseMeleeWeapon : public AFMBBaseWeapon
{
    GENERATED_BODY()
public:
    AFMBBaseMeleeWeapon();

    virtual void MeleeAttack(const EChooseAttack ChooseAttack) override;
    virtual void StopDrawTrace() override;

    FORCEINLINE UNiagaraComponent* GetSwordTrailFXComponent() const { return SwordTrailFXComponent; }

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "True"))
    UNiagaraComponent* SwordTrailFXComponent{nullptr};

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
    FName StartBladeTraceSocketName{"StartBladeTraceSocket"};

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
    FName EndBladeTraceSocketName{"EndBladeTraceSocket"};

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
    FName SwordTrailSocketName{"SwordTrailSocket"};

    UPROPERTY()
    float TraceRadius{10.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "True"))
    USoundCue* SwordSlashSound{nullptr};

    FTimerHandle DrawTraceTimerHandle;

    UPROPERTY()
    TArray<AActor*> HitActors;

    void StartDrawTrace();
    void DrawTrace();

    FVector FindBladeSocketLocation(const FName BladeTraceSocketName) const;
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;

    void SortEqualCharacter(const FHitResult& HitResult);
    void NewDamagedActor(const FHitResult& HitResult);
    void MakeDamage(const FHitResult& HitResult);

    void SpawnSwordSlashSound() const;

    virtual void UpdateItemInfoProperty(const AFMBPlayerCharacter* CurrentPlayerCharacter) const override;
};
