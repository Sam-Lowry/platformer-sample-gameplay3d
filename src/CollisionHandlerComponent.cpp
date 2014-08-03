#include "CollisionHandlerComponent.h"

#include "EnemyComponent.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "Messages.h"
#include "MessagesLevel.h"
#include "MessagesPlayer.h"
#include "TerrainInfo.h"

namespace platformer
{
    CollisionHandlerComponent::CollisionHandlerComponent()
        : Component(true)
        , _player(nullptr)
        , _playerClimbingTerrainRefCount(0)
        , _waitForPhysicsCleanup(false)
        , _framesSinceLevelReloaded(0)
        , _forceHandOfGodMessage(nullptr)
    {
    }

    CollisionHandlerComponent::~CollisionHandlerComponent()
    {
    }

    void CollisionHandlerComponent::onMessageReceived(gameplay::AIMessage * message)
    {
        switch (message->getId())
        {
        case(Messages::Type::LevelLoaded):
            onLevelUnloaded();
            onLevelLoaded();
            break;
        case(Messages::Type::LevelUnloaded):
            onLevelUnloaded();
            break;
        case(Messages::Type::PlayerNodeChangedMessage):
            {
                PlayerNodeChangedMessage msg(message);

                if(_playerCharacterNodes.insert(msg._currentNode).second)
                {
                    msg._currentNode->addRef();
                    msg._currentNode->getCollisionObject()->addCollisionListener(this);
                }

                _playerClimbingTerrainRefCount = 0;
                _player->setClimbingEnabled(false);
            }
            break;
        }
    }

    void CollisionHandlerComponent::onLevelLoaded()
    {
        _waitForPhysicsCleanup = true;
        std::vector<EnemyComponent *> enemyComponents;
        getParent()->getComponentsInChildren(enemyComponents);

        for(EnemyComponent * enemy  : enemyComponents)
        {
            enemy->addRef();
            gameplay::Node * enemyNode = enemy->getTerrainCollisionTriggerNode();
            enemyNode->addRef();
            gameplay::PhysicsCollisionObject * physics = enemyNode->getCollisionObject();
            physics->addCollisionListener(this);
            _enemies[physics] = enemy;
        }

        _player = getParent()->getComponentInChildren<PlayerComponent>();
        _player->addRef();
        gameplay::Node * playerCharacterNode = _player->getCharacterNode();
        playerCharacterNode->getCollisionObject()->addCollisionListener(this);
        playerCharacterNode->addRef();
        _playerCharacterNodes.insert(playerCharacterNode);
    }

    void CollisionHandlerComponent::onLevelUnloaded()
    {
        for(auto & enemyPair  : _enemies)
        {
            EnemyComponent * enemyComponent = enemyPair.second;
            gameplay::PhysicsCollisionObject * enemyPhysics = enemyPair.first;
            gameplay::Node * enemyNode = enemyPhysics->getNode();
            enemyPhysics->removeCollisionListener(this);
            SAFE_RELEASE(enemyNode);
            SAFE_RELEASE(enemyComponent);
        }

        _enemies.clear();

        for(gameplay::Node * node : _playerCharacterNodes)
        {
            node->getCollisionObject()->removeCollisionListener(this);
            SAFE_RELEASE(node);
        }

        _playerCharacterNodes.clear();

        SAFE_RELEASE(_player);
    }

    void CollisionHandlerComponent::update(float elapsedTime)
    {
        if(_waitForPhysicsCleanup)
        {
            ++_framesSinceLevelReloaded;

            if(_framesSinceLevelReloaded > 1)
            {
                _framesSinceLevelReloaded = 0;
                _waitForPhysicsCleanup = false;
            }
        }
    }

    void CollisionHandlerComponent::initialize()
    {
        _forceHandOfGodMessage = PlayerForceHandOfGodResetMessage::create();
    }

    void CollisionHandlerComponent::finalize()
    {
        onLevelUnloaded();
        PLATFORMER_SAFE_DELETE_AI_MESSAGE(_forceHandOfGodMessage);
    }

    void CollisionHandlerComponent::collisionEvent(gameplay::PhysicsCollisionObject::CollisionListener::EventType type,
                        gameplay::PhysicsCollisionObject::CollisionPair const & collisionPair,
                        gameplay::Vector3 const & contactPointA, gameplay::Vector3 const & contactPointB)
    {
        if(!_waitForPhysicsCleanup)
        {
            if(!onEnemyCollision(type, collisionPair))
            {
                onPlayerCollision(type, collisionPair, contactPointA, contactPointB);
            }
        }
    }

    bool CollisionHandlerComponent::onEnemyCollision(gameplay::PhysicsCollisionObject::CollisionListener::EventType type,
                                                     gameplay::PhysicsCollisionObject::CollisionPair const & collisionPair)
    {
        if(type == gameplay::PhysicsCollisionObject::CollisionListener::EventType::COLLIDING)
        {
            switch(collisionPair.objectB->getType())
            {
            case(gameplay::PhysicsCollisionObject::Type::RIGID_BODY):
                {
                    auto itr = _enemies.find(collisionPair.objectA);

                    if(itr != _enemies.end())
                    {
                        EnemyComponent * enemyComponent = itr->second;
                        enemyComponent->onTerrainCollision();
                        return true;
                    }
                }
                break;
            case(gameplay::PhysicsCollisionObject::Type::CHARACTER):
                {
                    getRootParent()->broadcastMessage(_forceHandOfGodMessage);
                    return true;
                }
                break;
            }


        }

        return false;
    }

    bool CollisionHandlerComponent::onPlayerCollision(gameplay::PhysicsCollisionObject::CollisionListener::EventType type,
                        gameplay::PhysicsCollisionObject::CollisionPair const & collisionPair,
                        gameplay::Vector3 const &, gameplay::Vector3 const &)
    {
        if(collisionPair.objectA == _player->getCharacterNode()->getCollisionObject() &&
            collisionPair.objectB->getType() == gameplay::PhysicsCollisionObject::Type::GHOST_OBJECT)
        {
            if(TerrainInfo * terrainInfo = TerrainInfo::getTerrainInfo(collisionPair.objectB->getNode()))
            {
                bool const isColliding = type == gameplay::PhysicsCollisionObject::CollisionListener::EventType::COLLIDING;

                if(terrainInfo->_tileType == TileType::LADDER)
                {
                    isColliding ? ++_playerClimbingTerrainRefCount : --_playerClimbingTerrainRefCount;
                    _playerClimbingTerrainRefCount = MATH_CLAMP(_playerClimbingTerrainRefCount, 0, std::numeric_limits<int>::max());
                    _player->setClimbingEnabled(_playerClimbingTerrainRefCount > 0);
                    return true;
                }
                else if(terrainInfo->_tileType == TileType::RESET && isColliding)
                {
                    getRootParent()->broadcastMessage(_forceHandOfGodMessage);
                    return true;
                }
            }
        }

        return false;
    }
}
