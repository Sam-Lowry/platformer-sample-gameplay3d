#include "CollisionObjectComponent.h"

#include "GameObject.h"
#include "Common.h"

namespace platformer
{
    CollisionObjectComponent::CollisionObjectComponent()
        : _node(nullptr)
    {
    }

    CollisionObjectComponent::~CollisionObjectComponent()
    {
    }

    void CollisionObjectComponent::initialize()
    {
        gameplay::Properties * physicsProperties = createProperties(_physics.c_str());
        _node = gameplay::Node::create(getId().c_str());
        _node->setCollisionObject(physicsProperties);
        getParent()->getNode()->addChild(_node);
        SAFE_DELETE(physicsProperties);
    }

    void CollisionObjectComponent::finalize()
    {
        getParent()->getNode()->removeChild(_node);
        SAFE_RELEASE(_node);
    }

    void CollisionObjectComponent::readProperties(gameplay::Properties & properties)
    {
        _physics = properties.getString("physics");
    }

    gameplay::Node * CollisionObjectComponent::getNode() const
    {
        return _node;
    }
}
