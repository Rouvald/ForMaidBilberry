// For Maid Bilberry Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FMBCoreTypes.h"
#include "FMBBaseItem.generated.h"

class UBoxComponent;
class USphereComponent;
class UWidgetComponent;
class AFMBPlayerCharacter;

UCLASS()
class FORMAIDBILBERRY_API AFMBBaseItem : public AActor
{
    GENERATED_BODY()

public:
    FOnItemStateChangedSignature OnItemStateChanged;

    AFMBBaseItem();

    virtual void Tick(float DeltaSeconds) override;

    FORCEINLINE UStaticMeshComponent* GetItemMesh() const { return ItemMesh; }

    void ThrowWeapon();

    // void StartItemInterping();

    // FORCEINLINE void SetIsRotateYaw (bool IsEnable) {bIsRotateYaw = IsEnable;}

protected:
    /*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
    USceneComponent* DefaultRootComponent{nullptr};*/

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* BoxCollision{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* AreaCollision{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
    UWidgetComponent* ItemInfoWidgetComponent{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
    UStaticMeshComponent* ItemMesh{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Properties")
    FName ItemName{"BaseItem"};

    EItemState CurrentItemState{EItemState::EIS_Pickup};

    UPROPERTY()
    TMap<EItemState, FItemProperties> ItemPropertiesMap;

    virtual void BeginPlay() override;

    virtual void SetItemInfo() const;

    void SetItemState(const EItemState NewItemState);

    void FillItemPropertiesMap();
    void SetItemProperties(const EItemState NewItemState) const;

    AFMBPlayerCharacter* GetPlayerCharacter() const;
    AController* GetController() const;

private:
    /*bool bIsRotateYaw{true};
    float RotationYaw{1.0f};*/

    FTimerHandle ThrowingTimerHandle;
    float WeaponFallingTime{0.3f};
    bool bIsWeaponFalling{false};

    UFUNCTION()
    void OnAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnAreaEndOverlap(
        UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void StopFalling /*()*/ (UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);
};
