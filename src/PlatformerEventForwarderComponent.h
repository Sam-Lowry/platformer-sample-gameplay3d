#ifndef PLATFORMER_EVENT_FORWARDER_COMPONENT_H
#define PLATFORMER_EVENT_FORWARDER_COMPONENT_H

#include "Component.h"

namespace platformer
{
    /**
     * Forwards relevant GameObject messages to the game
     *
     * @script{ignore}
    */
    class PlatformerEventForwarderComponent : public Component
    {
    public:
        explicit PlatformerEventForwarderComponent();
        ~PlatformerEventForwarderComponent();
    protected:
        virtual void onMessageReceived(gameplay::AIMessage * message) override;
    private:
        PlatformerEventForwarderComponent(PlatformerEventForwarderComponent const &);
    };
}

#endif
