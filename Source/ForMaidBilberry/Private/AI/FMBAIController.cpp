// For Maid Bilberry Game. All Rights Recerved


#include "AI/FMBAIController.h"
#include "AI/FMBAIBaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/FMBAIPerceptionComponent.h"

AFMBAIController::AFMBAIController()
{
    FMBAIPerceptionComponent = CreateDefaultSubobject<UFMBAIPerceptionComponent>("PerceptionComponent");
    SetPerceptionComponent(*FMBAIPerceptionComponent);
}

void AFMBAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto FMBAICharacter = Cast<AFMBAIBaseCharacter>(InPawn);
    if (FMBAICharacter)
    {
        RunBehaviorTree(FMBAICharacter->BehaviorTreeAsset);
    }
}

void AFMBAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    const auto AimActor = GetFocusOnActor();
    SetFocus(AimActor);
}

AActor* AFMBAIController::GetFocusOnActor() const
{
    if(!GetBlackboardComponent()) return nullptr;
    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnEnemyKeyName));
}
