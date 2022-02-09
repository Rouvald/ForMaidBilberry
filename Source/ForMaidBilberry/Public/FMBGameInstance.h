// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FMBCoreTypes.h"
#include "FMBGameInstance.generated.h"

class USoundClass;
/**
 *
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBGameInstance : public UGameInstance
{
    GENERATED_BODY()
public:
    FORCEINLINE FLevelData GetStartLevel() const { return StartLevel; }
    FORCEINLINE void SetStartLevel(const FLevelData& Data) { StartLevel = Data; }

    FORCEINLINE const TArray<FLevelData>& GetLevelsData() const { return LevelsData; }

    // FORCEINLINE FCharacterData GetStartCharacter() const { return StartCharacter; }
    // FORCEINLINE void SetStartCharacter(const FCharacterData& Data) { StartCharacter = Data; }

    // FORCEINLINE const TArray<FCharacterData>& GetCharactersData() const { return CharactersData; }

    FORCEINLINE FName GetMenuLevelName() const { return MenuLevelName; }

    void ToggleVolume() const;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TArray<FLevelData> LevelsData;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName MenuLevelName{NAME_None};

    /*UPROPERTY(EditDefaultsOnly, Category = "Game")
    TArray<FCharacterData> CharactersData;*/

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundClass* MasterSoundClass{nullptr};

private:
    FLevelData StartLevel;
    // FCharacterData StartCharacter;
};
