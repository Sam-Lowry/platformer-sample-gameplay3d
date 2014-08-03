#include "gameplay.h"
#include "GameObjectController.h"
#include "MessagesPlayer.h"

namespace platformer
{
    gameplay::AIMessage * PlayerJumpMessage::create()
    {
        return gameplay::AIMessage::create(Messages::Type::PlayerJump, "", "", 0);
    }

    gameplay::AIMessage * PlayerForceHandOfGodResetMessage::create()
    {
        return gameplay::AIMessage::create(Messages::Type::PlayerForceHandOfGodReset, "", "", 0);
    }

    /** @script{ignore} */
    struct PlayerNodeChangedMessagergs
    {
        enum Enum
        {
            IgnorePreviousNode,
            PreviousNode,
            CurrentNode,

            ArgCount
        };
    };

    PlayerNodeChangedMessage::PlayerNodeChangedMessage(gameplay::AIMessage * message)
        : _previousNode(nullptr)
        , _currentNode(nullptr)
    {
        GP_ASSERT(message->getId() == Messages::Type::PlayerNodeChangedMessage);

        if(!message->getBoolean(PlayerNodeChangedMessagergs::IgnorePreviousNode))
        {
            _previousNode = GameObjectController::getInstance().getScene()->findNode(message->getString(PlayerNodeChangedMessagergs::PreviousNode));
        }

        _currentNode = GameObjectController::getInstance().getScene()->findNode(message->getString(PlayerNodeChangedMessagergs::CurrentNode));
    }

    gameplay::AIMessage * PlayerNodeChangedMessage::create()
    {
        return gameplay::AIMessage::create(Messages::Type::PlayerNodeChangedMessage, "", "", PlayerNodeChangedMessagergs::ArgCount);
    }

    void PlayerNodeChangedMessage::setMessage(gameplay::AIMessage * message, gameplay::Node * previousNode, gameplay::Node * currentNode)
    {
        GP_ASSERT(message->getId() == Messages::Type::PlayerNodeChangedMessage);
        bool const ignorePreviousNode = !previousNode;
        message->setBoolean(PlayerNodeChangedMessagergs::IgnorePreviousNode, ignorePreviousNode);
        message->setString(PlayerNodeChangedMessagergs::PreviousNode, ignorePreviousNode ? "" : previousNode->getId());
        message->setString(PlayerNodeChangedMessagergs::CurrentNode, currentNode->getId());
    }
}
