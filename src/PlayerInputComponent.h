#ifndef PLATFORMER_INPUT_COMPONENT_H
#define PLATFORMER_INPUT_COMPONENT_H

#include "Component.h"
#include "PlayerComponent.h"

namespace gameplay
{
    class Gamepad;
}

namespace platformer
{
    struct KeyMessage;

    /**
     * Applies keyboard and gamepad input to a sibling PlayerComponent
     *
     * @script{ignore}
    */
    class PlayerInputComponent : public Component
    {
    public:
        explicit PlayerInputComponent();
        ~PlayerInputComponent();
        gameplay::Form * getGamepadForm() const;
    protected:
        virtual void initialize() override;
        virtual void finalize() override;
        virtual void update(float) override;
        virtual void onStart() override;
        virtual void onMessageReceived(gameplay::AIMessage * message);
    private:
        struct GamepadButtons
        {
            enum Enum
            {
                Jump,

                EnumCount
            };
        };

        PlayerInputComponent(PlayerInputComponent const &);
        bool isGamepadButtonPressed(GamepadButtons::Enum button) const;
        bool isGamepadButtonReleased(GamepadButtons::Enum button) const;

        void onKeyboardInput(KeyMessage keyMessage);

        PlayerComponent * _player;
        gameplay::Gamepad * _gamePad;
        bool _gamepadButtonState[GamepadButtons::EnumCount];
        bool _previousGamepadButtonState[GamepadButtons::EnumCount];
        int _gamepadButtonMapping[GamepadButtons::EnumCount];
        gameplay::Vector2 _joystickMovementDirections[PlayerComponent::MovementDirection::EnumCount];
        gameplay::Vector2 _previousJoystickValue;
    };
}

#endif
