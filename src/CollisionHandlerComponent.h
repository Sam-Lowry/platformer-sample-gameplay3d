#ifndef PLATFORMER_COLLISION_HANDLER_COMPONENT_H
#define PLATFORMER_COLLISION_HANDLER_COMPONENT_H

#include "Component.h"
#include "PhysicsRigidBody.h"

namespace gameplay
{
    class AIMessage;
}

namespace platformer
{
    class EnemyComponent;
    class PlayerComponent;

    /**
     * Handles collision events between characters and the terrain
     *
     * @script{ignore}
    */
    class CollisionHandlerComponent : public Component, public gameplay::PhysicsRigidBody::CollisionListener
    {
    public:
        explicit CollisionHandlerComponent();
        ~CollisionHandlerComponent();

        virtual void onMessageReceived(gameplay::AIMessage * message) override;
        virtual void update(float elapsedTime) override;
        virtual void initialize() override;
        virtual void finalize() override;
        virtual void collisionEvent(gameplay::PhysicsCollisionObject::CollisionListener::EventType type,
                            gameplay::PhysicsCollisionObject::CollisionPair const & collisionPair,
                            gameplay::Vector3 const &, gameplay::Vector3 const &) override;
    private:
        CollisionHandlerComponent(CollisionHandlerComponent const &);

        void onLevelLoaded();
        void onLevelUnloaded();

        bool onEnemyCollision(gameplay::PhysicsCollisionObject::CollisionListener::EventType type,
                              gameplay::PhysicsCollisionObject::CollisionPair const & collisionPair);

        bool onPlayerCollision(gameplay::PhysicsCollisionObject::CollisionListener::EventType type,
                            gameplay::PhysicsCollisionObject::CollisionPair const & collisionPair,
                            gameplay::Vector3 const & contactPointA, gameplay::Vector3 const & contactPointB);

        std::map<gameplay::PhysicsCollisionObject *, EnemyComponent *> _enemies;
        PlayerComponent * _player;
        std::set<gameplay::Node *> _playerCharacterNodes;
        gameplay::AIMessage * _forceHandOfGodMessage;
        int _playerClimbingTerrainRefCount;
        int _framesSinceLevelReloaded;
        bool _waitForPhysicsCleanup;
    };
}

#endif
