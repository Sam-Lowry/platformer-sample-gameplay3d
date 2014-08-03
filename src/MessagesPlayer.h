#ifndef PLATFORMER_PLAYER_MESSAGES_H
#define PLATFORMER_PLAYER_MESSAGES_H

#include "Messages.h"

namespace gameplay
{
    class AIMessage;
    class Node;
}

namespace platformer
{
    /** @script{ignore} */
    struct PlayerJumpMessage
    {
        static gameplay::AIMessage * create();
    };

    /** @script{ignore} */
    struct PlayerForceHandOfGodResetMessage
    {
        static gameplay::AIMessage * create();
    };

    /** @script{ignore} */
    struct PlayerNodeChangedMessage
    {
        PlayerNodeChangedMessage(gameplay::AIMessage * message);
        static gameplay::AIMessage * create();
        static void setMessage(gameplay::AIMessage * message, gameplay::Node * previousNode, gameplay::Node * currentNode);

        gameplay::Node * _previousNode;
        gameplay::Node * _currentNode;
    };
}

#endif
