#ifndef PLATFORMER_AUDIO_COMPONENT_H
#define PLATFORMER_AUDIO_COMPONENT_H

#include "Component.h"

namespace platformer
{
    struct PlayerJumpMessage;

    /**
     * Listens for audible player messages and triggers playback of audio sources
     *
     * @script{ignore}
    */
    class PlayerAudioComponent : public Component
    {
    public:
        explicit PlayerAudioComponent();
        ~PlayerAudioComponent();
    protected:
        virtual void initialize() override;
        virtual void finalize() override;
        virtual void onMessageReceived(gameplay::AIMessage * message) override;
        virtual void readProperties(gameplay::Properties & properties) override;
    private:
        PlayerAudioComponent(PlayerAudioComponent const &);

        void addAudioNode(std::string const & audioSourcePath);
        std::string _jumpAudioSourcePath;
        std::map<std::string, gameplay::Node *> _audioNodes;
    };
}

#endif
