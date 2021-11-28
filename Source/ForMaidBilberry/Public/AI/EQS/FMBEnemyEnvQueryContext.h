// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "FMBEnemyEnvQueryContext.generated.h"

/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API UFMBEnemyEnvQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()

public:
    virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
    FName EnemyActorKeyName = "EnemyActor";
};
