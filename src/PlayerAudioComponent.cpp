#include "PlayerAudioComponent.h"

#include "GameObjectController.h"
#include "MessagesPlayer.h"

namespace platformer
{
    PlayerAudioComponent::PlayerAudioComponent()
    {
    }

    PlayerAudioComponent::~PlayerAudioComponent()
    {
    }

    void PlayerAudioComponent::initialize()
    {
        addAudioNode(_jumpAudioSourcePath);
    }

    void PlayerAudioComponent::finalize()
    {
        for(auto & pair : _audioNodes)
        {
            gameplay::Node * node = pair.second;

            if(node->getParent())
            {
                node->getParent()->removeChild(node);
            }

            PLATFORMER_ASSERT_SINGLE_REF(node);
            SAFE_RELEASE(node);
        }
    }

    void PlayerAudioComponent::addAudioNode(std::string const & audioSourcePath)
    {
        gameplay::Node * node = gameplay::Node::create();
        gameplay::AudioSource * audioSource = gameplay::AudioSource::create(audioSourcePath.c_str());
        node->setAudioSource(audioSource);
        SAFE_RELEASE(audioSource);
        _audioNodes[audioSourcePath] = node;
    }

    void PlayerAudioComponent::onMessageReceived(gameplay::AIMessage * message)
    {
        switch (message->getId())
        {
        case(Messages::Type::PlayerJump):
            _audioNodes[_jumpAudioSourcePath]->getAudioSource()->play();
            break;
        case(Messages::Type::PlayerNodeChangedMessage):
            {
                gameplay::Node * jumpAudioNode = _audioNodes[_jumpAudioSourcePath];
                GameObjectController::getInstance().getScene()->getActiveCamera()->getNode()->addChild(jumpAudioNode);
            }
            break;
        }
    }

    void PlayerAudioComponent::readProperties(gameplay::Properties & properties)
    {
        _jumpAudioSourcePath = properties.getString("jump_sound");
    }
}
