#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

class FMBAnimUtils
{
public:
    template <typename T>
    static T* FindNotifyByClass(UAnimSequenceBase* Animation)
    {
        if (!Animation) return nullptr;

        const auto NotifyEvents = Animation->Notifies;
        for (const auto NotifyEvent : NotifyEvents)
        {
            if (NotifyEvent.Notify)
            {
                const auto AnimNotify = Cast<T>(NotifyEvent.Notify);
                if (AnimNotify)
                {
                    return AnimNotify;
                }
            }
            if (NotifyEvent.NotifyStateClass)
            {
                const auto AnimNotifyState = Cast<T>(NotifyEvent.NotifyStateClass);
                if (AnimNotifyState)
                {
                    return AnimNotifyState;
                }
            }
        }
        return nullptr;
    }
};
