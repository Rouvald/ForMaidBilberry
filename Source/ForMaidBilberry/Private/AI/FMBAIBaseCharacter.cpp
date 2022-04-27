// For Maid Bilberry Game. All Rights Reserved

#include "AI/FMBAIBaseCharacter.h"
#include "AI/FMBAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrainComponent.h"
#include "FMBHealthBarWidget.h"
#include "FMBHealthComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/FMBEnemySignWidget.h"

#include "Components/FMBBaseWeaponComponent.h"

AFMBAIBaseCharacter::AFMBAIBaseCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = AFMBAIController::StaticClass();

    HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
    HealthBarWidgetComponent->SetupAttachment(GetRootComponent());
    HealthBarWidgetComponent->SetRelativeLocation(FVector(20.0f, 8.0f, 110.0f));
    HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthBarWidgetComponent->SetDrawAtDesiredSize(true);

    EnemySignWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemySignNiagaraComponent"));
    EnemySignWidgetComponent->SetupAttachment(GetRootComponent());
    EnemySignWidgetComponent->SetRelativeLocation(FVector(20.0f, 8.0f, 130.0f));
    EnemySignWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    EnemySignWidgetComponent->SetDrawAtDesiredSize(true);

    WeaponComponent = CreateDefaultSubobject<UFMBBaseWeaponComponent>(TEXT("WeaponComponent"));
}

void AFMBAIBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    checkf(HealthBarWidgetComponent, TEXT("HealthBarWidgetComponent = nullptr"));
    checkf(EnemySignWidgetComponent, TEXT("EnemySignWidgetComponent = nullptr"));

    if (GetWorld())
    {
        GetWorldTimerManager().SetTimer(WidgetsVisibilityTimerHandle, this, &AFMBAIBaseCharacter::UpdateWidgetsVisibility, 0.1f, true);
    }
}

void AFMBAIBaseCharacter::OnHealthChange(float Health, float HealthDelta)
{
    Super::OnHealthChange(Health, HealthDelta);

    const auto HealthBarWidget{Cast<UFMBHealthBarWidget>(HealthBarWidgetComponent->GetUserWidgetObject())};
    if (!HealthBarWidget) return;

    const auto EnemySignWidget{Cast<UFMBEnemySignWidget>(EnemySignWidgetComponent->GetUserWidgetObject())};
    if (!EnemySignWidget) return;

    HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
    EnemySignWidget->SetEnemySignVisibility(FMath::IsNearlyZero(Health));
}

void AFMBAIBaseCharacter::OnDeath()
{
    Super::OnDeath();

    WeaponComponent->StopDrawTrace();

    const auto FMBController = Cast<AAIController>(Controller);
    if (FMBController && FMBController->BrainComponent)
    {
        FMBController->BrainComponent->Cleanup();
    }
}

void AFMBAIBaseCharacter::UpdateWidgetsVisibility() const
{
    if (!GetWorld() ||                                                 //
        !GetWorld()->GetFirstPlayerController() ||                     //
        !GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()) //
        return;

    const auto PlayerLocation{GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation()};
    const auto Distance{FVector::Distance(PlayerLocation, GetActorLocation())};
    HealthBarWidgetComponent->SetVisibility(Distance < HealthVisibleDistance, true);
    EnemySignWidgetComponent->SetVisibility(FMath::IsWithinInclusive(Distance, HealthVisibleDistance, EnemySignVisibleDistance), true);
}
