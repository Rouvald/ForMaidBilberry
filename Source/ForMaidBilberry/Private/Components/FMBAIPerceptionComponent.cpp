// For Maid Bilberry Game. All Rights Recerved

#include "Components/FMBAIPerceptionComponent.h"
#include "AIController.h"
#include "Perception/AISense_Sight.h"
#include "FMBUtils.h"
#include "Components/FMBHealthComponent.h"

AActor* UFMBAIPerceptionComponent::GetEnemyPlayer() const
{
    TArray<AActor*> PerceiveActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceiveActors);
    if (PerceiveActors.Num() == 0) return nullptr;

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    /*AActor* BestActor = nullptr;
    float BestDistance = MAX_FLT;*/
    for (const auto PerceiveActor : PerceiveActors)
    {
        const auto HealthComponent = FMBUtils::GetFMBPlayerComponent<UFMBHealthComponent>(PerceiveActor);
        const auto PerceivePawn = Cast<APawn>(PerceiveActor);

        if (HealthComponent && !HealthComponent->IsDead() && PerceivePawn->IsPlayerControlled())
        {
            return PerceiveActor;
            /*const auto CurrentDistance = (PerceiveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if(CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestActor = PerceiveActor;
            }*/
        }
    }
    return nullptr;
}
