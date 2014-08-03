#include "PlayerInputComponent.h"

#include "Common.h"
#include "GameObject.h"
#include "GameObjectController.h"
#include "MessagesInput.h"

namespace platformer
{
    PlayerInputComponent::PlayerInputComponent()
    {
    }

    PlayerInputComponent::~PlayerInputComponent()
    {
    }

    void PlayerInputComponent::onStart()
    {
        _player = getParent()->getComponent<PlayerComponent>();
        _player->addRef();
    }

    void PlayerInputComponent::initialize()
    {
        memset(_previousGamepadButtonState, 0, GamepadButtons::EnumCount * sizeof(bool));
        memset(_gamepadButtonState, 0, GamepadButtons::EnumCount * sizeof(bool));
        _joystickMovementDirections[PlayerComponent::MovementDirection::Up] = gameplay::Vector2(0, 1);
        _joystickMovementDirections[PlayerComponent::MovementDirection::Down] = gameplay::Vector2(0, -1);
        _joystickMovementDirections[PlayerComponent::MovementDirection::Left] = gameplay::Vector2(-1, 0);
        _joystickMovementDirections[PlayerComponent::MovementDirection::Right] = gameplay::Vector2(1, 0);
        _gamepadButtonMapping[GamepadButtons::Jump] = gameplay::Gamepad::BUTTON_A;
        _gamePad = gameplay::Game::getInstance()->getGamepad(0);
    }

    void PlayerInputComponent::finalize()
    {
        SAFE_RELEASE(_player);
    }

    void PlayerInputComponent::update(float)
    {
        for(int i = 0; i < GamepadButtons::EnumCount; ++i)
        {
            _gamepadButtonState[i] =_gamePad->isButtonDown(static_cast<gameplay::Gamepad::ButtonMapping>(_gamepadButtonMapping[i]));
        }

        if(isGamepadButtonPressed(GamepadButtons::Jump))
        {
            _player->jump();
        }

        gameplay::Vector2 joystickValue;
        _gamePad->getJoystickValues(0, &joystickValue);

        if(_previousJoystickValue != joystickValue)
        {
            gameplay::Vector2 joystickValueUnit = joystickValue;
            joystickValueUnit.normalize();

            float const maxDirectionDelta = cos(MATH_DEG_TO_RAD(25));

            for(int i = PlayerComponent::MovementDirection::None + 1; i < PlayerComponent::MovementDirection::EnumCount; ++i)
            {
                _player->setMovementEnabled(static_cast<PlayerComponent::MovementDirection::Enum>(i),
                                         joystickValueUnit.dot(_joystickMovementDirections[i]) > maxDirectionDelta,
                                         joystickValue.length());
            }
        }

        memcpy(_previousGamepadButtonState, _gamepadButtonState, sizeof(_previousGamepadButtonState));
        _previousJoystickValue = joystickValue;
    }

    bool PlayerInputComponent::isGamepadButtonPressed(GamepadButtons::Enum button) const
    {
        return _gamepadButtonState[button] && !_previousGamepadButtonState[button];
    }

    bool PlayerInputComponent::isGamepadButtonReleased(GamepadButtons::Enum button) const
    {
        return !_gamepadButtonState[button] && _previousGamepadButtonState[button];
    }

    gameplay::Form * PlayerInputComponent::getGamepadForm() const
    {
        return _gamePad->getForm();
    }

    void PlayerInputComponent::onMessageReceived(gameplay::AIMessage * message)
    {
        switch (message->getId())
        {
        case(Messages::Type::Key) :
            onKeyboardInput(std::move(KeyMessage(message)));
            break;
        }
    }

    void PlayerInputComponent::onKeyboardInput(KeyMessage keyMessage)
    {   
        if (keyMessage._event == gameplay::Keyboard::KeyEvent::KEY_PRESS ||
            keyMessage._event == gameplay::Keyboard::KeyEvent::KEY_RELEASE)
        {
            bool const enable = keyMessage._event == gameplay::Keyboard::KeyEvent::KEY_PRESS;

            switch(keyMessage._key)
            {
                case gameplay::Keyboard::Key::KEY_LEFT_ARROW:
                case gameplay::Keyboard::Key::KEY_A:
                _player->setMovementEnabled(PlayerComponent::MovementDirection::Left, enable);
                break;
            case gameplay::Keyboard::Key::KEY_RIGHT_ARROW:
            case gameplay::Keyboard::Key::KEY_D:
                _player->setMovementEnabled(PlayerComponent::MovementDirection::Right, enable);
                break;
            case gameplay::Keyboard::Key::KEY_UP_ARROW:
            case gameplay::Keyboard::Key::KEY_W:
                _player->setMovementEnabled(PlayerComponent::MovementDirection::Up, enable);
                break;
            case gameplay::Keyboard::Key::KEY_DOWN_ARROW:
            case gameplay::Keyboard::Key::KEY_S:
                _player->setMovementEnabled(PlayerComponent::MovementDirection::Down, enable);
                break;
            case gameplay::Keyboard::Key::KEY_SPACE:
                if(enable)
                {
                    _player->jump();
                }
                break;
            }
        }
    }
}
