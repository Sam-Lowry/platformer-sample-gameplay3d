#include "PlayerComponent.h"

#include "Common.h"
#include "CollisionObjectComponent.h"
#include "GameObject.h"
#include "MessagesPlayer.h"
#include "SpriteAnimationComponent.h"

namespace platformer
{
    PlayerComponent::PlayerComponent()
        : _isLeftFacing(false)
        , _movementDirection(MovementDirection::None)
        , _previousState(State::Idle)
        , _movementSpeed(5.0f)
        , _jumpHeight(1.0f)
        , _characterNode(nullptr)
        , _characterNormalNode(nullptr)
        , _characterDuckingNode(nullptr)
        , _movementScale(0.0f)
        , _jumpMessage(nullptr)
        , _nodeChangedMessage(nullptr)
        , _climbingEnabled(false)
    {
    }

    PlayerComponent::~PlayerComponent()
    {
    }

    void PlayerComponent::onStart()
    {
        _animations[State::Idle] = getParent()->findComponent<SpriteAnimationComponent>(_idleAnimComponentId);
        _animations[State::Walking] = getParent()->findComponent<SpriteAnimationComponent>(_walkAnimComponentId);
        _animations[State::Ducking] = getParent()->findComponent<SpriteAnimationComponent>(_duckAnimComponentId);
        _animations[State::Cowering] = getParent()->findComponent<SpriteAnimationComponent>(_cowerAnimComponentId);
        _animations[State::Jumping] = getParent()->findComponent<SpriteAnimationComponent>(_jumpAnimComponentId);
        _animations[State::Climbing] = getParent()->findComponent<SpriteAnimationComponent>(_climbingCharacterComponentId);

        _characterNormalNode = getParent()->findComponent<CollisionObjectComponent>(_normalCharacterComponentId)->getNode();
        _characterDuckingNode = getParent()->findComponent<CollisionObjectComponent>(_duckingCharacterComponentId)->getNode();
        _characterNormalNode->addRef();
        _characterDuckingNode->addRef();
        _characterDuckingNode->getCollisionObject()->asCharacter()->setPhysicsEnabled(false);
        _characterNode = _characterNormalNode;
        _jumpMessage = PlayerJumpMessage::create();
        _nodeChangedMessage = PlayerNodeChangedMessage::create();
        _state = State::Idle;
        PlayerNodeChangedMessage::setMessage(_nodeChangedMessage, nullptr, _characterNode);
        getParent()->broadcastMessage(_nodeChangedMessage);
    }

    void PlayerComponent::finalize()
    {
        _characterNode = nullptr;
        SAFE_RELEASE(_characterNormalNode);
        SAFE_RELEASE(_characterDuckingNode);
        PLATFORMER_SAFE_DELETE_AI_MESSAGE(_jumpMessage);
        PLATFORMER_SAFE_DELETE_AI_MESSAGE(_nodeChangedMessage);
    }

    void PlayerComponent::readProperties(gameplay::Properties & properties)
    {
        _idleAnimComponentId = properties.getString("idle_anim");
        _walkAnimComponentId = properties.getString("walk_anim");
        _duckAnimComponentId = properties.getString("duck_anim");
        _cowerAnimComponentId = properties.getString("cower_anim");
        _jumpAnimComponentId = properties.getString("jump_anim");
        _climbingCharacterComponentId = properties.getString("climb_anim");
        _movementSpeed = properties.getFloat("speed");
        _jumpHeight = properties.getFloat("jump_height");
        _normalCharacterComponentId = properties.getString("normal_physics");
        _duckingCharacterComponentId = properties.getString("ducking_physics");
    }

    void PlayerComponent::swapCharacterPhysics()
    {
        gameplay::PhysicsCharacter * previousCharacter = _characterNode->getCollisionObject()->asCharacter();
        gameplay::Vector3 previousVelocity = previousCharacter->getCurrentVelocity();
        gameplay::Vector3 previousTranslation = _characterNode->getTranslation();
        _characterNode->setTranslation(gameplay::Vector3::zero());
        previousCharacter->setPhysicsEnabled(false);
        previousCharacter->setVelocity(gameplay::Vector3::zero());

        float const heightOffset = ((_animations[_state]->getCurrentSpriteSrc().height -
                _animations[_previousState]->getCurrentSpriteSrc().height) * PLATFORMER_UNIT_SCALAR) / 2.0f;
        previousTranslation.y += heightOffset;

        _characterNode = _characterNode == _characterNormalNode ? _characterDuckingNode : _characterNormalNode;
        _characterNode->setTranslation(previousTranslation);
        gameplay::PhysicsCharacter * currentCharacter = _characterNode->getCollisionObject()->asCharacter();
        currentCharacter->setPhysicsEnabled(true);
        currentCharacter->setVelocity(previousVelocity);
        PlayerNodeChangedMessage::setMessage(_nodeChangedMessage, previousCharacter->getNode(), currentCharacter->getNode());
        getRootParent()->broadcastMessage(_nodeChangedMessage);
    }

    PlayerComponent::State::Enum PlayerComponent::getState() const
    {
        return _state;
    }

    gameplay::Vector2 PlayerComponent::getPosition() const
    {
        return gameplay::Vector2(_characterNode->getTranslation().x, _characterNode->getTranslation().y);
    }

    void PlayerComponent::forEachAnimation(std::function <bool(State::Enum, SpriteAnimationComponent *)> func)
    {
        for (auto & pair : _animations)
        {
            if (func(pair.first, pair.second))
            {
                break;
            }
        }
    }

    void PlayerComponent::update(float elapsedTime)
    {
        if(_characterNode->getCollisionObject()->asCharacter()->isPhysicsEnabled())
        {
            gameplay::Vector3 velocity = _characterNode->getCollisionObject()->asCharacter()->getCurrentVelocity();

            if(_movementDirection == MovementDirection::None || (_movementDirection & MovementDirection::Vertical) != MovementDirection::None)
            {
                if (velocity.y == 0.0f && velocity.x != 0.0f)
                {
                    velocity = gameplay::Vector3::zero();
                }
            }

            if (velocity.isZero())
            {
                if(_movementDirection == MovementDirection::Down)
                {
                    if(_previousState != State::Climbing)
                    {
                        _state = State::Ducking;

                        if(_previousState != State::Ducking)
                        {
                            swapCharacterPhysics();
                        }
                    }
                    else
                    {
                        _state = State::Idle;
                    }
                }
                else if(_movementDirection == MovementDirection::None || _movementDirection == MovementDirection::Up)
                {
                    _state = State::Idle;
                }
            }
            else
            {
                if(velocity.y == 0.0f && velocity.x != 0.0f)
                {
                    _state = State::Walking;
                }

                float const minFallVelocity = -0.2f;

                if(velocity.y < minFallVelocity && (_state == State::Idle || _state == State::Walking))
                {
                    _state = State::Cowering;
                }
            }

            if(velocity != _characterNode->getCollisionObject()->asCharacter()->getCurrentVelocity())
            {
                velocity.z = 0.0f;
                _characterNode->getCollisionObject()->asCharacter()->setVelocity(velocity);
            }
        }
        else
        {
            if(_state == State::Climbing)
            {
                float const elapsedTimeMs = elapsedTime / 1000.0f;
                float const verticalMovementSpeed = _movementSpeed / 2.0f;
                _characterNode->translateY((_movementScale * verticalMovementSpeed) * elapsedTimeMs);
            }
        }

        if(_state != _previousState)
        {
            _animations[_previousState]->stop();
            getCurrentAnimation()->play();
        }

        if(_state == State::Walking || _state == State::Climbing)
        {
            getCurrentAnimation()->setSpeed(_movementScale);
        }

        if(_previousState == State::Ducking && _state != State::Ducking)
        {
            swapCharacterPhysics();
        }

        _characterNode->setTranslationZ(0);

        _previousState = _state;
    }

    SpriteAnimationComponent * PlayerComponent::getCurrentAnimation()
    {
        return _animations[_state];
    }

    gameplay::Node * PlayerComponent::getCharacterNode() const
    {
        return _characterNode;
    }

    void PlayerComponent::setMovementEnabled(MovementDirection::Enum direction, bool enabled, float scale /* = 1.0f */)
    {
        if(enabled)
        {
            float const minDuckScale = 0.75f;

            if(direction != MovementDirection::Down || scale > minDuckScale)
            {
                _movementDirection = direction;

                if(direction == MovementDirection::Up && _climbingEnabled)
                {
                    float const velocityY =  _characterNode->getCollisionObject()->asCharacter()->getCurrentVelocity().y;
                    float const maxVelocityYDelta = 0.1f;

                    if(velocityY >= 0.0f && velocityY <= (velocityY + maxVelocityYDelta))
                    {
                        _state = State::Climbing;
                        _characterNode->getCollisionObject()->asCharacter()->setVelocity(gameplay::Vector3::zero());
                        _characterNode->getCollisionObject()->asCharacter()->setPhysicsEnabled(false);
                    }
                }
                else
                {
                    if(_state == State::Climbing)
                    {
                        _characterNode->getCollisionObject()->asCharacter()->setPhysicsEnabled(true);
                        _state = State::Idle;
                    }

                    if((direction & MovementDirection::Horizontal) == direction)
                    {
                        _isLeftFacing = direction == MovementDirection::Left;
                        float horizontalSpeed = _isLeftFacing ? -_movementSpeed : _movementSpeed;
                        horizontalSpeed *= scale;

                        _characterNode->getCollisionObject()->asCharacter()->setVelocity(horizontalSpeed, 0.0f, 0.0f);
                    }
                }

                _movementScale = scale;
            }
        }
        else
        {
            if(_movementDirection == direction)
            {
                if(direction == MovementDirection::Up)
                {
                    _movementScale = 0.0f;
                }
                else
                {
                    _movementDirection = MovementDirection::None;
                }
            }
        }
    }

    void PlayerComponent::setClimbingEnabled(bool enabled)
    {
        if(_climbingEnabled && !enabled && _state == State::Climbing)
        {
            _movementScale = 0.0f;
        }

        _climbingEnabled = enabled;
    }

    void PlayerComponent::jump(float scale)
    {
        if(_characterNode->getCollisionObject()->asCharacter()->getCurrentVelocity().y == 0.0f &&
                _state != State::Ducking && _state != State::Climbing)
        {
            _state = State::Jumping;
            _characterNode->getCollisionObject()->asCharacter()->setPhysicsEnabled(true);
            _characterNode->getCollisionObject()->asCharacter()->jump(_jumpHeight * scale);
            getParent()->broadcastMessage(_jumpMessage);
        }
    }

    bool PlayerComponent::IsLeftFacing() const
    {
        return _isLeftFacing;
    }
}
