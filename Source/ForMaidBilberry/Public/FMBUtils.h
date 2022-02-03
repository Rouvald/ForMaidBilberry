#pragma once

class FMBUtils
{
public:
    template <typename T> static T* GetFMBPlayerComponent(AActor* PlayerPawn)
    {
        if (!PlayerPawn) return nullptr;

        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
    }

    static FText TextFromInt(int32 Number) { return FText::FromString(FString::FromInt(Number)); }

    static bool GetTraceData(APawn* PlayerPawn, FVector& ViewLocation, FRotator& ViewRotation)
    {
        if (!PlayerPawn) return false;

        if (!PlayerPawn->GetController()) return false;

        PlayerPawn->GetController()->GetPlayerViewPoint(ViewLocation, ViewRotation);

        return true;
    }

    static bool AreBothBots(AController* Controller1, AController* Controller2)
    {
        if (!Controller1 || !Controller2 || Controller1 == Controller2) return false;
        return !Controller1->IsPlayerController() && !Controller2->IsPlayerController();
    }
};
