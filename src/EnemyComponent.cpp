#include "EnemyComponent.h"

#include "Common.h"
#include "CollisionObjectComponent.h"
#include "GameObject.h"
#include "SpriteAnimationComponent.h"

namespace platformer
{
    EnemyComponent::EnemyComponent()
        : _isLeftFacing(false)
        ,_wasPreviouslyLeftFacing (!_isLeftFacing)
        , _movementSpeed(5.0f)
        , _characterNode(nullptr)
        , _triggerNode(nullptr)
        , _state(State::Walking)
    {
    }

    EnemyComponent::~EnemyComponent()
    {
    }

    void EnemyComponent::onStart()
    {
        _animations[State::Walking] = getParent()->findComponent<SpriteAnimationComponent>(_walkAnimComponentId);
        _characterNode = getParent()->findComponent<CollisionObjectComponent>(_characterComponentId)->getNode();
        _characterNode->getCollisionObject()->asCharacter()->setVelocity(gameplay::Vector3::zero());
        _characterNode->addRef();

        _triggerNode = getParent()->findComponent<CollisionObjectComponent>(_triggerComponentId)->getNode();
        _triggerNode->rotateZ(MATH_DEG_TO_RAD(90));
        _triggerNode->addRef();

        getCurrentAnimation()->play();
    }

    void EnemyComponent::finalize()
    {
        SAFE_RELEASE(_characterNode);
        SAFE_RELEASE(_triggerNode);
    }

    void EnemyComponent::readProperties(gameplay::Properties & properties)
    {
        _walkAnimComponentId = properties.getString("walk_anim");
        _characterComponentId = properties.getString("physics");
        _triggerComponentId = properties.getString("collision_trigger");
        _movementSpeed = properties.getFloat("speed");
    }

    EnemyComponent::State::Enum EnemyComponent::getState() const
    {
        return _state;
    }

    gameplay::Vector2 EnemyComponent::getPosition() const
    {
        return gameplay::Vector2(_characterNode->getTranslation().x, _characterNode->getTranslation().y);
    }

    void EnemyComponent::forEachAnimation(std::function <bool(State::Enum, SpriteAnimationComponent *)> func)
    {
        for (auto & pair : _animations)
        {
            if (func(pair.first, pair.second))
            {
                break;
            }
        }
    }

    void EnemyComponent::onTerrainCollision()
    {
        _isLeftFacing = !_isLeftFacing;
    }

    void EnemyComponent::update(float)
    {
        gameplay::Vector3 const originalVelocity = _characterNode->getCollisionObject()->asCharacter()->getCurrentVelocity();
        gameplay::Vector3 velocity = originalVelocity;

        if(_wasPreviouslyLeftFacing != _isLeftFacing || velocity.isZero())
        {
            velocity.z = 0.0f;
            velocity.x = _isLeftFacing ? -_movementSpeed : _movementSpeed;
            _wasPreviouslyLeftFacing = _isLeftFacing;
        }

        if(velocity != originalVelocity)
        {
            _characterNode->getCollisionObject()->asCharacter()->setVelocity(velocity);
        }

        _characterNode->setTranslationZ(0);
        _triggerNode->setTranslation(_characterNode->getTranslation());
    }

    SpriteAnimationComponent * EnemyComponent::getCurrentAnimation()
    {
        return _animations[_state];
    }

    gameplay::Node * EnemyComponent::getTerrainCollisionTriggerNode() const
    {
        return _triggerNode;
    }

    bool EnemyComponent::IsLeftFacing() const
    {
        return _isLeftFacing;
    }
}
