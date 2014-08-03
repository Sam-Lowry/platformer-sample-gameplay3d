#include "GameObjectController.h"

#include "Component.h"

namespace platformer
{
    #define GAMEOBJECT_CALLBACK_SCOPE ScopedGameObjectCallback scopedGameObjectCallback;

    GameObjectController::GameObjectController()
        : _gameObjectTypeDir("res/gameobjects")
        , _scene(nullptr)
        , _processingGameObjectCallbacks(false)
    {
    }

    GameObjectController:: ~GameObjectController()
    {
    }

    GameObjectController & GameObjectController::getInstance()
    {
        static GameObjectController instance;
        return instance;
    }

    void GameObjectController::initialize()
    {
        PLATFORMER_ASSERT(!_scene, "GameObjectController has already been initialized");
        _scene = gameplay::Scene::load("res/scenes/gameobjects.scene");

        std::vector<std::string> files;
        gameplay::FileSystem::listFiles(_gameObjectTypeDir.c_str(), files);

        for (std::string const & file : files)
        {
            std::string const path = _gameObjectTypeDir + "/" + file;
            
            gameplay::Properties * gameObjectDef = createProperties(path.c_str());
            PLATFORMER_ASSERT(_gameObjectTypes.find(file) == _gameObjectTypes.end(), "Duplicate gameObject definition file '%s' found", path.c_str());

            GameObjectTypeInfo & gameObjectTypeInfo = _gameObjectTypes[file];

            while (gameObjectDef->getNextProperty())
            {
                gameObjectTypeInfo._gameObjects.push_back(gameObjectDef->getString());
            }

            while (gameplay::Properties * componentDef = gameObjectDef->getNextNamespace())
            {
                bool componentTypeFound = false;

                for (auto & componentTypePair : _componentTypes)
                {
                    if (componentTypePair.second._name == componentDef->getNamespace())
                    {
                        gameObjectTypeInfo._components.push_back(std::make_pair(componentTypePair.first, componentDef));
                        componentTypeFound = true;
                        break;
                    }
                }

                PLATFORMER_ASSERT(componentTypeFound, "Unknown component type '%s'", componentDef->getNamespace());
            }

            gameObjectTypeInfo._definition = gameObjectDef;
        }
    }

    void GameObjectController::finalize()
    {
        _scene->visit(this, &GameObjectController::removeGameObject, false);
        SAFE_RELEASE(_scene);

        for (auto & gameObjectTypePair : _gameObjectTypes)
        {
            GameObjectTypeInfo & gameObjectTypeInfo = gameObjectTypePair.second;
            SAFE_DELETE(gameObjectTypeInfo._definition);
        }

        removeGameObjectsFromPreviousFrame();
        _gameObjectTypes.clear();
        _componentTypes.clear();
    }

    void GameObjectController::independentUpdate(float elapsedTime)
    {
        removeGameObjectsFromPreviousFrame();
        GAMEOBJECT_CALLBACK_SCOPE
        _scene->visit(this, &GameObjectController::independentUpdateGameObject, elapsedTime);
    }

    void GameObjectController::update(float elapsedTime)
    {
        GAMEOBJECT_CALLBACK_SCOPE
        _scene->visit(this, &GameObjectController::updateGameObject, elapsedTime);
    }

    void GameObjectController::updatePostPhysics(float elapsedTime)
    {
        GAMEOBJECT_CALLBACK_SCOPE
        _scene->visit(this, &GameObjectController::updateGameObjectPostPhysics, elapsedTime);
    }

    void GameObjectController::render(float elapsedTime)
    {
        GAMEOBJECT_CALLBACK_SCOPE
        _scene->visit(this, &GameObjectController::renderGameObject, elapsedTime);
    }

    void GameObjectController::broadcastGameObjectMessage(gameplay::AIMessage * message)
    {
        GAMEOBJECT_CALLBACK_SCOPE
        _scene->visit(this, &GameObjectController::broadcastGameObjectMessage, message);
    }

    void GameObjectController::broadcastGameObjectMessage(gameplay::AIMessage * message, GameObject * gameObject)
    {
        GAMEOBJECT_CALLBACK_SCOPE
        gameObject->broadcastMessage(message, gameObject->getNode());
    }

    GameObject * GameObjectController::createGameObject(std::string const & typeName)
    {
        gameplay::Node * nullNode = nullptr;
        return createGameObject(typeName, nullNode);
    }

    GameObject * GameObjectController::createGameObject(std::string const & typeName, GameObject * parent)
    {
        return createGameObject(typeName, parent->getNode());
    }

    GameObject * GameObjectController::createGameObject(std::string const & typeName, gameplay::Node * parentNode)
    {
        auto gameObjectTypeItr = _gameObjectTypes.find(typeName + ".go");
        PLATFORMER_ASSERT(gameObjectTypeItr != _gameObjectTypes.end(), "Unknown gameObject type '%s", typeName.c_str());
        GameObjectTypeInfo & gameObjectDef = gameObjectTypeItr->second;
        
        GameObject * gameObject = new GameObject();
        gameplay::Node * node = gameplay::Node::create(typeName.c_str());
        node->setUserPointer(gameObject);
        parentNode ? parentNode->addChild(node) : _scene->addNode(node);
        gameObject->_node = node;

        for (auto & gameObjectDefPair : gameObjectDef._components)
        {
            auto componentTypeItr = _componentTypes.find(gameObjectDefPair.first);
            ComponentTypeInfo const & componentTypeInfo = componentTypeItr->second;
            Component * component = componentTypeInfo._generator();
            gameplay::Properties & properties = *gameObjectDefPair.second;
            std::string id = properties.getId();

            if (id.empty())
            {
                char buffer[UCHAR_MAX];
                sprintf(buffer, "annon_%d", static_cast<int>(gameObject->_components.size()));
                id = buffer;
            }

            component->_id = id;
            component->readProperties(*gameObjectDefPair.second);
            component->_typeId = gameObjectDefPair.first;
            component->_parent = gameObject;
            component->initialize();
            gameObject->addComponent(component);
        }

        for (std::string & gameObjectTypeName : gameObjectDef._gameObjects)
        {
            createGameObject(gameObjectTypeName, node);
        }
        
        gameObject->forEachComponent([](Component * component) -> bool
        {
            component->onStart();
            return false;
        });

        return  gameObject;
    }

    void GameObjectController::destroyGameObject(GameObject * gameObject)
    {
        removeGameObject(gameObject, true);
    }

    void GameObjectController::removeGameObjectsFromPreviousFrame()
    {
        if(!_componentsToRemoveNextFrame.empty())
        {
            for(Component * component : _componentsToRemoveNextFrame)
            {
                SAFE_RELEASE(component);
            }

            _componentsToRemoveNextFrame.clear();
        }
    }

    bool GameObjectController::removeGameObject(gameplay::Node * node, bool removeNode)
    {
        if (GameObject * gameObject = GameObject::getGameObject(node))
        {
            removeGameObject(gameObject, removeNode);
        }
        return true;
    }

    void GameObjectController::removeGameObject(GameObject * gameObject, bool removeNode)
    {
        if(!_processingGameObjectCallbacks)
        {
            gameplay::Node * node = gameObject->_node;

            gameObject->forEachComponent([this](Component * component) -> bool
            {
                if(component->_waitForNextFrameBeforeDeletion)
                {
                    component->addRef();
                    _componentsToRemoveNextFrame.insert(component);
                }

                return false;
            });

            gameObject->finalize();
            node->setUserPointer(nullptr);

            if (removeNode)
            {
                _scene->removeNode(node);
            }

            SAFE_RELEASE(gameObject);
        }
        else
        {
            _gameObjectsToRemove.insert(gameObject);
        }
    }

    void GameObjectController::preGameObjectCallbacks()
    {
        _processingGameObjectCallbacks = true;
    }

    void GameObjectController::postGameObjectCallbacks()
    {
        _processingGameObjectCallbacks = false;

        if(! _gameObjectsToRemove.empty())
        {
            for(auto gameObjectToRemoveItr = _gameObjectsToRemove.begin(); gameObjectToRemoveItr != _gameObjectsToRemove.end(); ++gameObjectToRemoveItr)
            {
                removeGameObject(*gameObjectToRemoveItr, true);
            }

            _gameObjectsToRemove.clear();
        }
    }

    bool GameObjectController::updateGameObject(gameplay::Node * node, float elapsedTime)
    {
        if (GameObject * gameObject = GameObject::getGameObject(node))
        {
            gameObject->update(elapsedTime);
        }

        return true;
    }

    bool GameObjectController::updateGameObjectPostPhysics(gameplay::Node * node, float elapsedTime)
    {
        if (GameObject * gameObject = GameObject::getGameObject(node))
        {
            gameObject->updatePostPhysics(elapsedTime);
        }

        return true;
    }

    bool GameObjectController::independentUpdateGameObject(gameplay::Node * node, float elapsedTime)
    {
        if (GameObject * gameObject = GameObject::getGameObject(node))
        {
            gameObject->independentUpdate(elapsedTime);
        }

        return true;
    }

    bool GameObjectController::renderGameObject(gameplay::Node * node, float elapsedTime)
    {
        if (GameObject * gameObject = GameObject::getGameObject(node))
        {
            gameObject->render(elapsedTime);
        }

        return true;
    }

    bool GameObjectController::broadcastGameObjectMessage(gameplay::Node * node, gameplay::AIMessage * message)
    {
        if (GameObject * gameObject = GameObject::getGameObject(node))
        {
            gameObject->broadcastMessage(message, node);
        }

        return true;
    }

    gameplay::Scene * GameObjectController::getScene() const
    {
        return _scene;
    }

    GameObjectController::ScopedGameObjectCallback::ScopedGameObjectCallback()
    {
        GameObjectController::getInstance().preGameObjectCallbacks();
    }

    GameObjectController::ScopedGameObjectCallback::~ScopedGameObjectCallback()
    {
        GameObjectController::getInstance().postGameObjectCallbacks();
    }


#ifndef _FINAL
    void GameObjectController::renderDebug(float elapsedTime, gameplay::Font * font)
    {
        RenderDebugArgs args { elapsedTime, font };
        _scene->visit(this, &GameObjectController::renderGameObjectDebug, &args);
    }

    bool GameObjectController::renderGameObjectDebug(gameplay::Node * node, RenderDebugArgs * renderDebugArgs)
    {
        if (GameObject * gameObject = GameObject::getGameObject(node))
        {
            gameObject->renderDebug(renderDebugArgs->_elapsedTime, renderDebugArgs->_font);
        }

        return true;
    }
#endif
}
