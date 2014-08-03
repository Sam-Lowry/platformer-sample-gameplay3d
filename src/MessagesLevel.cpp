#include "MessagesLevel.h"

#include "gameplay.h"
#include "Messages.h"

namespace platformer
{
    gameplay::AIMessage * LevelLoadedMessage::create()
    {
        return gameplay::AIMessage::create(Messages::Type::LevelLoaded, "", "", 0);
    }

    gameplay::AIMessage * LevelUnloadedMessage::create()
    {
        return gameplay::AIMessage::create(Messages::Type::LevelUnloaded, "", "", 0);
    }

    gameplay::AIMessage * RequestLevelReloadMessage::create()
    {
        return gameplay::AIMessage::create(Messages::Type::RequestLevelReloadMessage, "", "", 0);
    }
}
