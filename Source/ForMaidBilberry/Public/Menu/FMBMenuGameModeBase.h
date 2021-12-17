// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FMBCoreTypes.h"
#include "FMBMenuGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API AFMBMenuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
    FOnMenuStateChangeSignature OnMenuStateChange;
    
    AFMBMenuGameModeBase();

    virtual void StartPlay() override;

    void SetMenuState(EFMBMenuState State);

    //void SetCharacterSkeletalMesh(USkeletalMesh* NewSkeletalMesh);
    
private:
    EFMBMenuState MenuState = EFMBMenuState::WaitingToStart;
};
