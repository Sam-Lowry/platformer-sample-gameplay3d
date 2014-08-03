#ifndef PLATFORMER_COLLISION_OBJECT_COMPONENT_H
#define PLATFORMER_COLLISION_OBJECT_COMPONENT_H

#include "Component.h"

namespace gameplay
{
    class Properties;
}

namespace platformer
{
    /**
     * Creates a collision object from a .physics file and creates a node for it.
     * The created node is added as a child node to the parent GameObject node.
     *
     * @script{ignore}
    */
    class CollisionObjectComponent : public Component
    {
    public:
        explicit CollisionObjectComponent();
        ~CollisionObjectComponent();

        virtual void initialize() override;
        virtual void finalize() override;
        virtual void readProperties(gameplay::Properties & properties) override;

        gameplay::Node * getNode() const;
    private:
        CollisionObjectComponent(CollisionObjectComponent const &);

        std::string _physics;
        gameplay::Node * _node;
    };
}

#endif
