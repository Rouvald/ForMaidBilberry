#pragma once

#include "FMBCoreTypes.generated.h"

// Animation

class UAnimSequence;
class UAnimMontage;

USTRUCT(BlueprintType)
struct FWeaponAnimationsData
{
    GENERATED_USTRUCT_BODY()

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

UENUM(BlueprintType)
enum class EChooseAttack: uint8
{
    FastAttack = 0,
    StrongAttack
};

// Stamina
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStaminaChangeSignature, float);

USTRUCT(BlueprintType)
struct FStaminaSpendData
{
    GENERATED_USTRUCT_BODY()

    //UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spend Stamina")
    //float JumpStaminaSpend = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spend Stamina")
    float RollingStaminaSpend = 15.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spend Stamina")
    float FastAttackStaminaSpend = 30.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spend Stamina")
    float StrongAttackStaminaSpend = 50.0f;
};

UENUM(BlueprintType)
enum class EStaminaSpend: uint8
{
    Rolling = 0,
    FastAttack,
    StrongAttack
};

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

// GameMode

USTRUCT(BlueprintType)
struct FGameData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game", meta =(ClampMin ="1", ClampMax ="50"))
    int32 PlayerNum = 2;

    /*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game", meta =(ClampMin ="1", ClampMax ="10"))
    int32 RoundsNum = 4;*/

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game")
    bool InfinityGame = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game",
        meta = (EditCondition = "!InfinityGame", ClampMin ="1", ClampMax ="400"))
    int32 GameplayTime = 10; // in second

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game", meta =(ClampMin ="3", ClampMax ="30"))
    int32 RespawnTime = 10; // in second

    /*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game")
    USkeletalMesh* DefaultTeamSkeletalMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game")
    TArray<USkeletalMesh*> TeamSkeletalMeshes;*/
};
