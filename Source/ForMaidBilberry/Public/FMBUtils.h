﻿#pragma once

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
};
