#pragma once

#include "FMBCoreTypes.generated.h"

// Animation

class UAnimSequence;
class UAnimMontage;

USTRUCT(BlueprintType)
struct FWeaponAnimationsData
{
    GENERATED_USTRUCT_BODY()

    /*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation")
    UAnimSequence* Idle;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation")
    UAnimSequence* Run;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation")
    UAnimSequence* Sprint;*/

    UPROPERTY(EditDefaultsOnly, Category="Weapon", meta=(ToolTip="2 sockets: WeaponEquipSocket_R or WeaponEquipSocket_L"))
    FName WeaponEquipSocketName; // WeaponEquipSocket_R or WeaponEquipSocket_L

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation")
    UAnimSequence* Jump_Start;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation")
    UAnimSequence* Jump_Air;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation")
    UAnimSequence* Jump_End;

    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage* Roll;

    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage* GetHit;

    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage* Equip;

    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage* FastAttack;

    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage* StrongAttack;
};

// Weapon

UENUM(BlueprintType)
enum class EWeaponType: uint8
{
    RedSword = 0,
    YellowSword
};

// Stamina
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStaminaChangeSignature, float);

// Health
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangeSignature, float, float);

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

UENUM(BlueprintType)
enum class EChooseAttack: uint8
{
    FastAttack = 0,
    StrongAttack
};

UENUM(BlueprintType)
enum class EStaminaSpend: uint8
{
    Rolling = 0,
    FastAttack,
    StrongAttack
};
