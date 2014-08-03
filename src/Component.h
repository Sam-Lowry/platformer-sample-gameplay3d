#ifndef PLATFORMER_COMPONENT_H
#define PLATFORMER_COMPONENT_H

#include "Common.h"
#include <typeindex>

namespace platformer
{
    class GameObject;

    /**
     * A component enapsulates some aspect of gameplay logic. A component can only be created as part of a GameObject
     * file (*.go). A component declaration within a GameObject may define serializable properties.
     *
     * The re-usability of the component is up to the implementation of the derrived class. A component may be completley
     * de-coupled from other components by using the message passing system. Alternatively, a component may depend on other
     * components at various level of the GameObject hierarchy. Ideally, any inter-dependency between components should be
     * kept to a minimum and should avoid being bi-directional.
     *
     * @script{ignore}
    */
    class Component : public gameplay::Ref
    {
        friend class GameObject;
        friend class GameObjectController;

    public:
        explicit Component(bool waitForNextFrameBeforeDeletion = false);
        virtual ~Component() {}

        GameObject * getParent();

        GameObject * getRootParent();

        /**
         * Returns an id that uniquely identifies this component from its sibling components.
         */
        std::string const & getId() const;
    protected:

        /**
         * Read messages that were broadcast to the parent GameObject here.
         */
        virtual void onMessageReceived(gameplay::AIMessage * message) {}

        /**
         * Read properties defined in the component declaration here.
         *
         * This will be invoked before initialize
         */
        virtual void readProperties(gameplay::Properties & properties) {}

        /**
         * Run any initialization here that depends on serialized properties but not on
         * other sibling components.
         *
         * This will be invoked after readProperties
         */
        virtual void initialize() {}

        /**
         * Run any initialization here that depends on sibling components/GameObject ancestors.
         *
         * This will be invoked after readProperties and initialize
         */
        virtual void onStart() {}

        /**
         * Release resources and shared references here.
         */
        virtual void finalize() {}

        /**
         * Run any logic here that isn't bound to the games timestep
         *
         * Examples of this could be network updates or updates during pause
         */
        virtual void independentUpdate(float elapsedTime) {}

        /**
         * Run any logic here that is bound to the games timestep.
         *
         * Any modification of the games physics state shold be done here, see updatePostPhysics.
         *
         * This method will not be invoked during pause and elapsedTime may be negative
         * with games that feature some form of rewind mechanic
         */
        virtual void update(float elapsedTime) {}

        /**
         * Run any logic that relies on other components having modified any aspect of
         * the physics state during update
         *
         * Note that this method and update will always be invoked after Gameplay has internally updated
         * its own physics system. This purpose of this method is to faciliate the synchronisation of
         * physics state among all components per frame.
         */
        virtual void updatePostPhysics(float elapsedTime) {}

        /**
         * Render to the display
         *
         * Aim to keep any rendering operations localised to as few components as possible in order to
         * take advantage of batching/grouping effects
         */
        virtual void render(float elapsedTime) {}

#ifndef _FINAL
        virtual void renderDebug(float elapsedTime, gameplay::Font * font) {}
#endif
    private:
        Component(Component const &);

        std::string _id;
        std::type_index _typeId;
        GameObject * _parent;
        bool _waitForNextFrameBeforeDeletion;
    };
}

#endif
