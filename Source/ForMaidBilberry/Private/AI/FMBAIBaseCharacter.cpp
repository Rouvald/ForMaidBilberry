// For Maid Bilberry Game. All Rights Recerved

#include "AI/FMBAIBaseCharacter.h"
#include "AI/FMBAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrainComponent.h"
#include "FMBHealthBarWidget.h"
#include "FMBHealthComponent.h"
#include "Components/WidgetComponent.h"

AFMBAIBaseCharacter::AFMBAIBaseCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = AFMBAIController::StaticClass();

    HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthBarWidgetComponent");
    HealthBarWidgetComponent->SetupAttachment(GetRootComponent());
    HealthBarWidgetComponent->SetRelativeLocation(FVector(20.0f, 8.0f, 110.0f));
    HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthBarWidgetComponent->SetDrawAtDesiredSize(true);
}

void AFMBAIBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthBarWidgetComponent);

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            HealthBarVisibleTimerHandle, this, &AFMBAIBaseCharacter::UpdateHealthBarWidgetVisible, 0.5f, true);
    }
}

void AFMBAIBaseCharacter::OnHealthChange(float Health, float HealthDelta)
{
    Super::OnHealthChange(Health, HealthDelta);

    const auto HealthBarWidget = Cast<UFMBHealthBarWidget>(HealthBarWidgetComponent->GetUserWidgetObject());
    if (!HealthBarWidget) return;

    HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
}

void AFMBAIBaseCharacter::OnDeath()
{
    Super::OnDeath();

    const auto FMBController = Cast<AAIController>(Controller);
    if (FMBController && FMBController->BrainComponent)
    {
        FMBController->BrainComponent->Cleanup();
    }
}

void AFMBAIBaseCharacter::UpdateHealthBarWidgetVisible() const
{
    if (!GetWorld() ||                                                 //
        !GetWorld()->GetFirstPlayerController() ||                     //
        !GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()) //
        return;

    const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
    const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());
    HealthBarWidgetComponent->SetVisibility(Distance < HealthVisibleDistance, true);
}
