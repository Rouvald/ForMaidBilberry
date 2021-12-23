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
    FLevelData GetStartLevel() const { return StartLevel; }
    void SetStartLevel(const FLevelData& Data) { StartLevel = Data; }

    const TArray<FLevelData>& GetLevelsData() const { return LevelsData; }

    FCharacterData GetStartCharacter() const { return StartCharacter; }
    void SetStartCharacter(const FCharacterData& Data) { StartCharacter = Data; }

    const TArray<FCharacterData>& GetCharactersData() const { return CharactersData; }

    FName GetMenuLevelName() const { return MenuLevelName; }

    void ToggleVolume() const;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TArray<FLevelData> LevelsData;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName MenuLevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TArray<FCharacterData> CharactersData;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundClass* MasterSoundClass;

private:
    FLevelData StartLevel;

    FCharacterData StartCharacter;
};
