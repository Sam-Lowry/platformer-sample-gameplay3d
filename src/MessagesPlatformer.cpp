#include "MessagesPlatformer.h"

#include "gameplay.h"

namespace platformer
{
    /** @script{ignore} */
    struct PlatformerSplashScreenChangeRequestMessageArgs
    {
        enum Enum
        {
            Increment,
            ApplyOverTime,

            ArgCount
        };
    };

    PlatformerSplashScreenChangeRequestMessage::PlatformerSplashScreenChangeRequestMessage(gameplay::AIMessage * message)
        : _increment(0.0f)
        , _applyOverTime(true)
    {
        GP_ASSERT(message->getId() == Messages::Type::PlatformerSplashScreenChangeRequestMessage);
        _increment = message->getFloat(PlatformerSplashScreenChangeRequestMessageArgs::Increment);
        _applyOverTime = message->getBoolean(PlatformerSplashScreenChangeRequestMessageArgs::ApplyOverTime);
    }

    gameplay::AIMessage * PlatformerSplashScreenChangeRequestMessage::create()
    {
        return gameplay::AIMessage::create(Messages::Type::PlatformerSplashScreenChangeRequestMessage, "", "", PlatformerSplashScreenChangeRequestMessageArgs::ArgCount);
    }

    void PlatformerSplashScreenChangeRequestMessage::setMessage(gameplay::AIMessage * message, float increment, bool applyOverTime)
    {
        GP_ASSERT(message->getId() == Messages::Type::PlatformerSplashScreenChangeRequestMessage);
        message->setFloat(PlatformerSplashScreenChangeRequestMessageArgs::Increment, increment);
        message->setBoolean(PlatformerSplashScreenChangeRequestMessageArgs::ApplyOverTime, applyOverTime);
    }
}
