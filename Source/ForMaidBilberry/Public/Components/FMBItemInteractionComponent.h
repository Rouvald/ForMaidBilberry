// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FMBCoreTypes.h"
#include "FMBItemInteractionComponent.generated.h"

class AFMBBaseItem;
class AFMBPlayerCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FORMAIDBILBERRY_API UFMBItemInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    FOnItemAreaOverlapSignature OnItemAreaOverlap;

    UFMBItemInteractionComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    FORCEINLINE AFMBBaseItem* GetHitItem() const { return HitItem; }
    /* @note: Not good decision, but OK, i think.*/
    FORCEINLINE void ClearHitItem() { HitItem = nullptr; }

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    AFMBPlayerCharacter* PlayerCharacter{nullptr};

    /* Interaction with Items*/
    FTimerHandle ItemInfoVisibilityTimerHandle;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    float ItemVisibilityTraceDistance = 5000.0f;

    UPROPERTY()
    AFMBBaseItem* HitItem{nullptr};

    UPROPERTY()
    TArray<const AFMBBaseItem*> HittedItems;
    //

    void ItemInfoVisibilityTimer(const AFMBBaseItem* Item, bool bIsOverlap);
    void UpdateItemInfoVisibility();
    bool MakeHitItemVisibility(FHitResult& HitResult) const;
    void HideAllHittedItems() const;

    AFMBPlayerCharacter* GetPlayerCharacter() const;
};
