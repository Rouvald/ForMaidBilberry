// For Maid Bilberry Game. All Rights Recerved


#include "AI/FMBAIBaseCharacter.h"
#include "AI/FMBAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AFMBAIBaseCharacter::AFMBAIBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit)
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = AFMBAIController::StaticClass();

    /*if(GetCharacterMovement())
    {
         GetCharacterMovement()->bOrientRotationToMovement = false;
    }*/

}
