#ifndef PLATFORMER_INPUT_MESSAGES_H
#define PLATFORMER_INPUT_MESSAGES_H

#include "gameplay.h"
#include "Messages.h"

namespace platformer
{
    /** @script{ignore} */
    struct KeyMessage
    {
        KeyMessage(gameplay::AIMessage * message);
        static gameplay::AIMessage * create();
        static void setMessage(gameplay::AIMessage * message, gameplay::Keyboard::KeyEvent _event, int _key);

        gameplay::Keyboard::KeyEvent _event;
        int _key;
    };

    /** @script{ignore} */
    struct TouchMessage
    {
        TouchMessage(gameplay::AIMessage * message);
        static gameplay::AIMessage * create();
        static void setMessage(gameplay::AIMessage * message, gameplay::Touch::TouchEvent event, int x, int y, int contactIndex);

        gameplay::Touch::TouchEvent _event;
        int _x;
        int _y;
        unsigned int _contactIndex;
    };

    /** @script{ignore} */
    struct MouseMessage
    {
        MouseMessage(gameplay::AIMessage * message);
        static gameplay::AIMessage * create();
        static void setMessage(gameplay::AIMessage * message, gameplay::Mouse::MouseEvent event, int x, int y, int wheelDelta);

        gameplay::Mouse::MouseEvent _event;
        int _x;
        int _y;
        int _wheelDelta;
    };

    /** @script{ignore} */
    struct GamepadMessage
    {
        GamepadMessage(gameplay::AIMessage * message);
        static gameplay::AIMessage * create();
        static void setMessage(gameplay::AIMessage * message, gameplay::Gamepad::GamepadEvent event, int gamepadIndex);

        gameplay::Gamepad::GamepadEvent _event;
        int _gamepadIndex;
    };
}

#endif
