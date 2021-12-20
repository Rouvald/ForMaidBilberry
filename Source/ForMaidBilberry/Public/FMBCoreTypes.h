#pragma once

#include "FMBCoreTypes.generated.h"

// Animation

class UAnimSequence;
class UAnimMontage;

USTRUCT(BlueprintType)
struct FWeaponAnimationsData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ToolTip = "2 sockets: WeaponEquipSocket_R or WeaponEquipSocket_L"))
    FName WeaponEquipSocketName; // WeaponEquipSocket_R or WeaponEquipSocket_L

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
    UAnimSequence* Jump_Start;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
    UAnimSequence* Jump_Air;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
    UAnimSequence* Jump_End;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* Roll;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* GetHit;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* Equip;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* FastAttack;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* StrongAttack;
};

// Weapon

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    RedSword = 0,
    YellowSword
};

UENUM(BlueprintType)
enum class EChooseAttack : uint8
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

    // UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spend Stamina")
    // float JumpStaminaSpend = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spend Stamina")
    float RollingStaminaSpend = 15.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spend Stamina")
    float FastAttackStaminaSpend = 30.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spend Stamina")
    float StrongAttackStaminaSpend = 50.0f;
};

UENUM(BlueprintType)
enum class EStaminaSpend : uint8
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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    UTexture2D* WeaponIcon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FName WeaponName = NAME_None;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponSelectedSignature, const FWeaponUIData&);

// GameMode

USTRUCT(BlueprintType)
struct FGameData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "50"))
    int32 PlayerNum = 2;

    /*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game", meta =(ClampMin ="1", ClampMax ="10"))
    int32 RoundsNum = 4;*/

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    bool InfinityGame = false;

<<<<<<< HEAD
=======
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    int32 GameOverDelayTime = 5;

>>>>>>> 63640ef (Rework gamepver condition. Clear some buggs.)
    UPROPERTY(
        EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (EditCondition = "!InfinityGame", ClampMin = "1", ClampMax = "400"))
    int32 GameplayTime = 10; // in second

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "30"))
    int32 RespawnTime = 10; // in second

    /*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game")
    USkeletalMesh* DefaultTeamSkeletalMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game")
    TArray<USkeletalMesh*> TeamSkeletalMeshes;*/
};

UENUM(BlueprintType)
enum class EFMBMatchState : uint8
{
    WaitingToStart = 0,
    InProgress,
    Pause,
    GameOver
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChangeSignature, EFMBMatchState);

// Game

USTRUCT(BlueprintType)
struct FLevelData
{
    GENERATED_USTRUCT_BODY()

    int32 LevelID = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName LevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName LevelDisplayName = NAME_None;

    /*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game")
    FText LevelDescriptionText;*/

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    UTexture2D* LevelImage;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelSelectedSignature, const FLevelData&);

// Menu

UENUM(BlueprintType)
enum class EFMBMenuState : uint8
{
    WaitingToStart = 0,
    MainMenu,
    SelectLevel,
    ChangeSkin
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMenuStateChangeSignature, EFMBMenuState);

// change character
USTRUCT(BlueprintType)
struct FCharacterData
{
    GENERATED_USTRUCT_BODY()

    int32 CharacterID = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName CharacterName = NAME_None;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    USkeletalMesh* CharacterMesh;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterMeshSelectedSignature, const FCharacterData&);
//
