// For Maid Bilberry Game. All Rights Recerved


#include "AI/FMBAIBaseCharacter.h"
#include "AI/FMBAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrainComponent.h"

AFMBAIBaseCharacter::AFMBAIBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit)
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = AFMBAIController::StaticClass();

    /*if(GetCharacterMovement())
    {
         GetCharacterMovement()->bOrientRotationToMovement = false;
    }*/

}

void AFMBAIBaseCharacter::OnDeath()
{
    Super::OnDeath();

    const auto FMBController = Cast<AAIController>(Controller);
    if(FMBController && FMBController->BrainComponent)
    {
        FMBController->BrainComponent->Cleanup();
    }
}
