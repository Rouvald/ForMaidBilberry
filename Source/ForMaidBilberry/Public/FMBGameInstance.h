// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FMBCoreTypes.h"
#include "FMBGameInstance.generated.h"

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
    
    FName GetMenuLevelName() const { return MenuLevelName; }

protected:
    UPROPERTY(EditDefaultsOnly, Category="Game")
    TArray<FLevelData> LevelsData;

    UPROPERTY(EditDefaultsOnly, Category="Game")
    FName MenuLevelName = NAME_None;

private:
    FLevelData StartLevel;
};
