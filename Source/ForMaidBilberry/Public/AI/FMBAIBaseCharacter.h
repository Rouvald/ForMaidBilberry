// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "Player/FMBBaseCharacter.h"
#include "FMBAIBaseCharacter.generated.h"

class UBehaviorTree;
/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API AFMBAIBaseCharacter : public AFMBBaseCharacter
{
	GENERATED_BODY()

public:
    AFMBAIBaseCharacter (const FObjectInitializer& ObjInit);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
    UBehaviorTree* BehaviorTreeAsset;
};
