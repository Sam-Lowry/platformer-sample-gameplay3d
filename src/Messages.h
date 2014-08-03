#ifndef PLATFORMER_MESSAGES_H
#define PLATFORMER_MESSAGES_H

namespace platformer
{
    /**
     * Defines unique Id's for all game object message types used in the platformer
     *
     * @script{ignore}
    */
    struct Messages
    {
        enum Type
        {
            Key,
            Mouse,
            Touch,
            Gamepad,
            PlayerJump,
            PlayerForceHandOfGodReset,
            PlayerNodeChangedMessage,
            LevelLoaded,
            LevelUnloaded,
            RequestLevelReloadMessage,
            PlatformerSplashScreenChangeRequestMessage
        };
    };
}

#endif
