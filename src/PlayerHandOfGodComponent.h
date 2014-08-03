#ifndef PLATFORMER_PLAYER_HOG_COMPONENT_H
#define PLATFORMER_PLAYER_HOG_COMPONENT_H

#include "Component.h"

namespace platformer
{
    class PlayerComponent;

    /**
     * Moves the player to the spawn position when they venture outside the boundaries of the current level
     *
     * @script{ignore}
    */
    class PlayerHandOfGodComponent : public Component
    {
    public:
        explicit PlayerHandOfGodComponent();
        ~PlayerHandOfGodComponent();
    protected:
        virtual void update(float elapsedTime) override;
        virtual void initialize() override;
        virtual void finalize() override;
        virtual void onStart() override;
        virtual void onMessageReceived(gameplay::AIMessage * message) override;
        virtual void readProperties(gameplay::Properties & properties) override;
    private:
        gameplay::Vector2 _resetPosition;
        gameplay::Vector2 _boundaryScale;
        gameplay::Rectangle _boundary;
        PlayerComponent * _player;
        bool _levelLoaded;
        bool _forceNextReset;
        gameplay::AIMessage * _splashScreenFadeMessage;
    };
}

#endif
