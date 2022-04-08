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
    FName WeaponEquipSocketName = "WeaponEquipSocket_R"; // WeaponEquipSocket_R or WeaponEquipSocket_L

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
    UAnimSequence* Jump_Start{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
    UAnimSequence* Jump_Air{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
    UAnimSequence* Jump_End{nullptr};

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* Roll{nullptr};

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* GetHit{nullptr};

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* Equip{nullptr};

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* FastAttack{nullptr};

    /*UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* JumpAttack{nullptr};*/

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* StrongAttack{nullptr};
};

// Item

class AFMBBaseItem;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemAreaOverlapSignature, const AFMBBaseItem*, bool);

// Weapon

class UNiagaraSystem;
class USoundCue;

USTRUCT(BlueprintType)
struct FImpactData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* NiagaraEffect{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* ImpactSound{nullptr};
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    EWT_RedSword UMETA(DisplayName = "RedSword"),
    EWT_YellowSword UMETA(DisplayName = "YellowSword"),

    EWT_Max UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class EChooseAttack : uint8
{
    ECA_FastAttack UMETA(DisplayName = "ESS_FastAttack"),
    ECA_StrongAttack UMETA(DisplayName = "ESS_StrongAttack"),

    ECA_Max UMETA(DisplayName = "Max")
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
    float RollingStaminaSpend{15.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spend Stamina")
    float FastAttackStaminaSpend{30.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spend Stamina")
    float StrongAttackStaminaSpend{50.0f};
};

UENUM(BlueprintType)
enum class EStaminaSpend : uint8
{
    ESS_Rolling UMETA(DisplayName = "Rolling"),
    ESS_FastAttack UMETA(DisplayName = "FastAttack"),
    ESS_StrongAttack UMETA(DisplayName = "StrongAttack"),

    ESS_Max UMETA(DisplayName = "Max")
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
    UTexture2D* WeaponIcon{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FName WeaponName{NAME_None};
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponSelectedSignature, const FWeaponUIData&);

// GameMode

// Sky

UENUM(BlueprintType)
enum class EDayTime : uint8
{
    EDT_Day UMETA(DisplayName = "Day"),
    // EDT_Morning UMETA(DisplayName = "Morning"),
    // EDT_Evening UMETA(DisplayName = "Evening"),
    EDT_Night UMETA(DisplayName = "Night"),

    EDT_Max UMETA(DisplayName = "Max")
};

DECLARE_MULTICAST_DELEGATE(FOnChangeSunRotationSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDayNightChangeSignature, const EDayTime);

USTRUCT(BlueprintType)
struct FGameData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    bool bIsWalkAlone{false};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game",
        meta = (EditCondition = "!bIsWalkAlone", ClampMin = "1", ClampMax = "50", ToolTip = "For normal game need PlayerNum > 1"))
    int32 PlayerNum{2};

    // For true, need check bWantsPlayerState = true in FMBAIController
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Game")
    bool CanBotsRespawn{false};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "30"))
    int32 RespawnTime{5}; // in second

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sky")
    EDayTime DefaultDayTime{EDayTime::EDT_Day};

    UPROPERTY(EditDefaultsOnly, Category = "Sky", meta = (ClampMin = "5", ClampMax = "60", ToolTip = "Day in minutes"))
    float MaxDayMinuteTime{30.0f}; // in minutes
};

UENUM(BlueprintType)
enum class EGameState : uint8
{
    EGS_WaitingToStart UMETA(DisplayName = "WaitingToStart"),
    EGS_InProgress UMETA(DisplayName = "InProgress"),
    EGS_Pause UMETA(DisplayName = "Pause"),
    EGS_GameOver UMETA(DisplayName = "GameOver"),

    EGS_Max UMETA(DisplayName = "Max")
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChangeSignature, EGameState);

// Game

USTRUCT(BlueprintType)
struct FLevelData
{
    GENERATED_USTRUCT_BODY()

    int32 LevelID = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName LevelName{NAME_None};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName LevelDisplayName{NAME_None};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName LevelDescriptionName{NAME_None};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    UTexture2D* LevelImage{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FGameData GameData;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelSelectedSignature, const FLevelData&);

// Menu

UENUM(BlueprintType)
enum class EMenuState : uint8
{
    EMS_WaitingToStart UMETA(DisplayName = "WaitingToStart"),
    EMS_MainMenu UMETA(DisplayName = "MainMenu"),
    EMS_SelectLevel UMETA(DisplayName = "SelectLevel"),
    EMS_ChangeSkin UMETA(DisplayName = "ChangeSkin"),

    EMS_Max UMETA(DisplayName = "Max")
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMenuStateChangeSignature, EMenuState);

// change character
/*
 *USTRUCT(BlueprintType)
struct FCharacterData
{
    GENERATED_USTRUCT_BODY()

    int32 CharacterID{0};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    FName CharacterName{NAME_None};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
    USkeletalMesh* CharacterMesh{nullptr};
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterMeshSelectedSignature, const FCharacterData&);
*/
//
