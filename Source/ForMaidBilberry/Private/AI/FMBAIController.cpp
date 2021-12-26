// For Maid Bilberry Game. All Rights Recerved

#include "AI/FMBAIController.h"
#include "AI/FMBAIBaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/FMBAIPerceptionComponent.h"
#include "Components/FMBRespawnComponent.h"

AFMBAIController::AFMBAIController()
{
    FMBAIPerceptionComponent = CreateDefaultSubobject<UFMBAIPerceptionComponent>("PerceptionComponent");
    SetPerceptionComponent(*FMBAIPerceptionComponent);

    RespawnComponent = CreateDefaultSubobject<UFMBRespawnComponent>("RespawnComponent");
    // uncomment for respawns bots
    // bWantsPlayerState = true;
}

void AFMBAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto FMBAICharacter = Cast<AFMBAIBaseCharacter>(InPawn);
    if (FMBAICharacter)
    {
        RunBehaviorTree(FMBAICharacter->BehaviorTreeAsset);
        if (GetBlackboardComponent())
        {
            GetBlackboardComponent()->SetValueAsVector(StartAILocationKeyName, FMBAICharacter->GetActorLocation());
        }
    }
}

void AFMBAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    const auto EnemyActor = GetFocusOnActor();
    SetFocus(EnemyActor);
}

AActor* AFMBAIController::GetFocusOnActor() const
{
    if (!GetBlackboardComponent()) return nullptr;
    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnEnemyKeyName));
}
