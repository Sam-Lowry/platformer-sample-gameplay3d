#ifndef PLATFORMER_GAMEOBJECTCONTROLLER_H
#define PLATFORMER_GAMEOBJECTCONTROLLER_H

#include "Common.h"
#include <typeindex>

#include "GameObject.h"

namespace platformer
{
    /**
     * Defines a class for creating, destroying and broadcasting messages to game objects. All component
     * types must be registered with this controller in order to be serializable from game object files.
     *
     * The Game class should be responsible for forwarding platform level events to game objects as well as
     * invoking the various update and render methods.
     */
    class GameObjectController
    {
        friend class ScopedGameObjectCallback;

    public:
        /**
         * @return GameObjectController
         */
        static GameObjectController & getInstance();

        /** @script{ignore} */
        void initialize();

        /** @script{ignore} */
        void finalize();

        /** @script{ignore} */
        void broadcastGameObjectMessage(gameplay::AIMessage * message);

        /** @script{ignore} */
        void broadcastGameObjectMessage(gameplay::AIMessage * message, GameObject * gameObject);

        /** @script{ignore} */
        void independentUpdate(float elapsedTime);

        /** @script{ignore} */
        void update(float elapsedTime);

        /** @script{ignore} */
        void updatePostPhysics(float elapsedTime);

        /** @script{ignore} */
        void render(float elapsedTime);

#ifndef _FINAL
        /** @script{ignore} */
        void renderDebug(float elapsedTime, gameplay::Font * font);
#endif
        /**
         * @param gameObject
         */
        void destroyGameObject(GameObject * gameObject);

        /**
         * @param typeName
         */
        GameObject * createGameObject(std::string const & typeName);

        /**
         * @param typeName
         * @param parentNode
         */
        GameObject * createGameObject(std::string const & typeName, gameplay::Node * parentNode);

        /**
         * @param typeName
         * @param parent
         * @return GameObject
         */
        GameObject * createGameObject(std::string const & typeName, GameObject * parent);

        template<typename ComponentType>
        /** @script{ignore} */
        void registerComponent(std::string const & name);

        /** @script{ignore} */
        gameplay::Scene * getScene() const;
    private:
        struct ScopedGameObjectCallback
        {
            ScopedGameObjectCallback();
            ~ScopedGameObjectCallback();
        };

        explicit GameObjectController();
        ~GameObjectController();
        GameObjectController(GameObjectController const &);

        void removeGameObjectsFromPreviousFrame();
        bool removeGameObject(gameplay::Node * node, bool removeNode);
        void removeGameObject(GameObject * gameObject, bool removeNode);
        bool broadcastGameObjectMessage(gameplay::Node * node, gameplay::AIMessage * message);
        bool independentUpdateGameObject(gameplay::Node * _node, float elapsedTime);
        bool updateGameObject(gameplay::Node * node, float elapsedTime);
        bool updateGameObjectPostPhysics(gameplay::Node * node, float elapsedTime);
        bool renderGameObject(gameplay::Node * node, float elapsedTime);
#ifndef _FINAL
        struct RenderDebugArgs
        {
            float _elapsedTime;
            gameplay::Font * _font;
        };

        bool renderGameObjectDebug(gameplay::Node * node, RenderDebugArgs * renderDebugArgs);
#endif

        void preGameObjectCallbacks();
        void postGameObjectCallbacks();

        struct ComponentTypeInfo
        {
            std::string _name;
            std::function<Component*()> _generator;
        };

        struct GameObjectTypeInfo
        {
            std::vector<std::pair<std::type_index, gameplay::Properties *>> _components;
            std::vector<std::string> _gameObjects;
            gameplay::Properties * _definition;
        };

        gameplay::Scene * _scene;
        std::string _gameObjectTypeDir;
        std::set<Component *> _componentsToRemoveNextFrame;
        std::map<std::type_index, ComponentTypeInfo> _componentTypes;
        std::map<std::string, GameObjectTypeInfo> _gameObjectTypes;
        std::set<GameObject *> _gameObjectsToRemove;
        bool _processingGameObjectCallbacks;
    };
}

#include "GameObjectController.inl"

#endif
