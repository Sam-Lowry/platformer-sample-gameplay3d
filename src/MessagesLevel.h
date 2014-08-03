#ifndef PLATFORMER_LEVEL_MESSAGES_H
#define PLATFORMER_LEVEL_MESSAGES_H

namespace gameplay
{
    class AIMessage;
}

namespace platformer
{
    /** @script{ignore} */
    struct LevelLoadedMessage
    {
        static gameplay::AIMessage * create();
    };

    /** @script{ignore} */
    struct LevelUnloadedMessage
    {
        static gameplay::AIMessage * create();
    };

    /** @script{ignore} */
    struct RequestLevelReloadMessage
    {
        static gameplay::AIMessage * create();
    };
}

#endif
