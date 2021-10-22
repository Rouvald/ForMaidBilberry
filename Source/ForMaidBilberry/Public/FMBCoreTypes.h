#pragma once

#include "FMBCoreTypes.generated.h"

// Stamina
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStaminaChangeSignature, float);

// Health
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangeSignature, float);

// UI
USTRUCT(BlueprintType)
struct FWeaponUIData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
    UTexture2D* WeaponIcon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
    FText WeaponName;
};