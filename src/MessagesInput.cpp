#include "MessagesInput.h"

namespace platformer
{
    /** @script{ignore} */
    struct KeyMessageArgs
    {
        enum Enum
        {
            Event,
            Key,

            ArgCount
        };
    };

    KeyMessage::KeyMessage(gameplay::AIMessage * message)
    {
        GP_ASSERT(message->getId() == Messages::Type::Key);
        _event = static_cast<gameplay::Keyboard::KeyEvent>(message->getInt(KeyMessageArgs::Event));
        _key = message->getInt(KeyMessageArgs::Key);
    }

    gameplay::AIMessage * KeyMessage::create()
    {
        return gameplay::AIMessage::create(Messages::Type::Key, "", "", KeyMessageArgs::ArgCount);
    }

    void KeyMessage::setMessage(gameplay::AIMessage * message, gameplay::Keyboard::KeyEvent event, int key)
    {
        GP_ASSERT(message->getId() == Messages::Type::Key);
        message->setInt(KeyMessageArgs::Event, event);
        message->setInt(KeyMessageArgs::Key, key);
    }

    /** @script{ignore} */
    struct TouchMessageArgs
    {
        enum Enum
        {
            Event,
            X,
            Y,
            ContactIndex,

            ArgCount
        };
    };

    TouchMessage::TouchMessage(gameplay::AIMessage * message)
    {
        GP_ASSERT(message->getId() == Messages::Type::Touch);
        _event = static_cast<gameplay::Touch::TouchEvent>(message->getInt(TouchMessageArgs::Event));
        _x = message->getInt(TouchMessageArgs::X);
        _y = message->getInt(TouchMessageArgs::Y);
        _contactIndex = message->getInt(TouchMessageArgs::ContactIndex);
    }

    gameplay::AIMessage * TouchMessage::create()
    {
        return gameplay::AIMessage::create(Messages::Type::Touch, "", "", TouchMessageArgs::ArgCount);
    }

    void TouchMessage::setMessage(gameplay::AIMessage * message, gameplay::Touch::TouchEvent event, int x, int y, int contactIndex)
    {
        GP_ASSERT(message->getId() == Messages::Type::Touch);
        message->setInt(TouchMessageArgs::Event, event);
        message->setInt(TouchMessageArgs::X, x);
        message->setInt(TouchMessageArgs::Y, y);
        message->setInt(TouchMessageArgs::ContactIndex, contactIndex);
    }

    /** @script{ignore} */
    struct MouseMessageArgs
    {
        enum Enum
        {
            Event,
            X,
            Y,
            WheelDelta,

            ArgCount
        };
    };

    MouseMessage::MouseMessage(gameplay::AIMessage * message)
    {
        GP_ASSERT(message->getId() == Messages::Type::Mouse);
        _event = static_cast<gameplay::Mouse::MouseEvent>(message->getInt(MouseMessageArgs::Event));
        _x = message->getInt(MouseMessageArgs::X);
        _y = message->getInt(MouseMessageArgs::Y);
        _wheelDelta = message->getInt(MouseMessageArgs::WheelDelta);
    }

    gameplay::AIMessage * MouseMessage::create()
    {
        return gameplay::AIMessage::create(Messages::Type::Mouse, "", "", MouseMessageArgs::ArgCount);
    }

    void MouseMessage::setMessage(gameplay::AIMessage * message, gameplay::Mouse::MouseEvent event, int x, int y, int wheelDelta)
    {
        GP_ASSERT(message->getId() == Messages::Type::Mouse);
        message->setInt(MouseMessageArgs::Event, event);
        message->setInt(MouseMessageArgs::X, x);
        message->setInt(MouseMessageArgs::Y, y);
        message->setInt(MouseMessageArgs::WheelDelta, wheelDelta);
    }

    /** @script{ignore} */
    struct GamepadMessageArgs
    {
        enum Enum
        {
            Event,
            GamepadIndex,

            ArgCount
        };
    };

    GamepadMessage::GamepadMessage(gameplay::AIMessage * message)
    {
        GP_ASSERT(message->getId() == Messages::Type::Gamepad);
        _event = static_cast<gameplay::Gamepad::GamepadEvent>(message->getInt(GamepadMessageArgs::Event));
        _gamepadIndex = message->getInt(GamepadMessageArgs::GamepadIndex);
    }

    gameplay::AIMessage * GamepadMessage::create()
    {
        return gameplay::AIMessage::create(Messages::Type::Gamepad, "", "", GamepadMessageArgs::ArgCount);
    }

    void GamepadMessage::setMessage(gameplay::AIMessage * message, gameplay::Gamepad::GamepadEvent event, int gamepadIndex)
    {
        GP_ASSERT(message->getId() == Messages::Type::Gamepad);
        message->setInt(GamepadMessageArgs::Event, event);
        message->setInt(GamepadMessageArgs::GamepadIndex, gamepadIndex);
    }
}
