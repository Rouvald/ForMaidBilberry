#pragma once

class FMBUtils
{
public:
    static void AttachItemToSocket(AActor* Item, USceneComponent* MeshComp, const FName& SocketName)
    {
        if (!Item || !MeshComp) return;

        const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
        Item->AttachToComponent(MeshComp, AttachmentRules, SocketName);
    }

    template <typename T> static T* GetFMBPlayerComponent(AActor* PlayerPawn)
    {
        if (!PlayerPawn) return nullptr;

        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
    }

    static FText TextFromInt(const int32 Number) { return FText::FromString(FString::FromInt(Number)); }

    static bool GetTraceData(const APawn* PlayerPawn, FVector& TraceStart, FVector& TraceEnd, const float TraceDistance)
    {
        if (!PlayerPawn || !PlayerPawn->GetController()) return false;

        FVector ViewLocation;
        FRotator ViewRotation;

        PlayerPawn->GetController()->GetPlayerViewPoint(ViewLocation, ViewRotation);

        TraceStart = ViewLocation;
        const auto ShootDirection = ViewRotation.Vector();
        TraceEnd = TraceStart + ShootDirection * TraceDistance;
        return true;
    }

    static bool AreBothBots(const AController* Controller1, const AController* Controller2)
    {
        if (!Controller1 || !Controller2 || Controller1 == Controller2) return false;
        return !Controller1->IsPlayerController() && !Controller2->IsPlayerController();
    }
};
