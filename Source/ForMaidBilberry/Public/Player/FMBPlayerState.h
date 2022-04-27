// For Maid Bilberry Game. All Rights Reserved

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

    FORCEINLINE int32 GetKillsNum() const { return KillsNum; }
    FORCEINLINE void AddKill() { ++KillsNum; }

    FORCEINLINE int32 GetDeathsNum() const { return DeathsNum; }
    FORCEINLINE void AddDeath() { ++DeathsNum; }

    void LogInfo();

private:
    /*int32 TeamID;

    UPROPERTY()
    USkeletalMesh* TeamSkeletalMesh;*/

    int32 KillsNum{0};
    int32 DeathsNum{0};
};
