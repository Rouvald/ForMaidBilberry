// For Maid Bilberry Game. All Rights Recerved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FMBGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class FORMAIDBILBERRY_API AFMBGameHUD : public AHUD
{
	GENERATED_BODY()

    public:
    virtual void DrawHUD() override;

    private:
    void DrawTwoLine();
};
