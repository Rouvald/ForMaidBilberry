// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FMBPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API AFMBPlayerState : public APlayerState
{
    GENERATED_BODY()
public:
    /*void SetTeamID(int32 NewTeamID) { TeamID = NewTeamID;}
    int32 GetTeamID() const {return TeamID;}

    void SetTeamSkeletalMesh(USkeletalMesh* NewTeamSkeletalMesh) { TeamSkeletalMesh = NewTeamSkeletalMesh;}
    USkeletalMesh* GetTeamSkeletalMesh() const {return TeamSkeletalMesh;}*/

    int32 GetKillsNum() const { return KillsNum;}
    void AddKill() { ++KillsNum; }

    int32 GetDeathsNum() const { return DeathsNum;}
    void AddDeath() { ++DeathsNum; }

    void LogInfo();

private:
    /*int32 TeamID;

    UPROPERTY()
    USkeletalMesh* TeamSkeletalMesh;*/

    int32 KillsNum = 0;
    int32 DeathsNum = 0;
};
