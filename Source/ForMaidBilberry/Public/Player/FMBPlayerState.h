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
    void SetTeamID(int32 NewTeamID) { TeamID = NewTeamID;}
    int32 GetTeamID() const {return TeamID;}

    void SetTeamColor(USkeletalMesh* NewTeamSkeletalMesh) { TeamSkeletalMesh = NewTeamSkeletalMesh;}
    USkeletalMesh* GetTeamColor() const {return TeamSkeletalMesh;}
    
private:
    int32 TeamID;

    UPROPERTY()
    USkeletalMesh* TeamSkeletalMesh;
};
