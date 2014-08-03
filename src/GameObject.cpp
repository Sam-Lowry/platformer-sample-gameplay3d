#include "GameObject.h"
#include "GameObjectController.h"

#include "Component.h"

namespace platformer
{
    GameObject::GameObject()
        : _node(nullptr)
    {
    }

    GameObject::~GameObject(){}

    void GameObject::initialize()
    {
        for (auto & componentPair : _components)
        {
            for (Component * component : componentPair.second)
            {
                component->initialize();
            }
        }
    }

    void GameObject::finalize()
    {
        for (auto & componentPair : _components)
        {
            for (Component * component : componentPair.second)
            {
                component->finalize();
            }
        }

        for (auto & componentPair : _components)
        {
            for (Component * component : componentPair.second)
            {
                SAFE_RELEASE(component);
            }
        }

        SAFE_RELEASE(_node);
    }

    void GameObject::onMessageReceived(gameplay::AIMessage * message)
    {
        for (auto & componentPair : _components)
        {
            for (Component * component : componentPair.second)
            {
                component->onMessageReceived(message);
            }
        }
    }

    void GameObject::update(float elapsedTime)
    {
        for (auto & componentPair : _components)
        {
            for (Component * component : componentPair.second)
            {
                component->update(elapsedTime);
            }
        }
    }

    void GameObject::updatePostPhysics(float elapsedTime)
    {
        for (auto & componentPair : _components)
        {
            for (Component * component : componentPair.second)
            {
                component->updatePostPhysics(elapsedTime);
            }
        }
    }

    void GameObject::render(float elapsedTime)
    {
        for (auto & componentPair : _components)
        {
            for (Component * component : componentPair.second)
            {
                component->render(elapsedTime);
            }
        }
    }

    void GameObject::independentUpdate(float elapsedTime)
    {
        for (auto & componentPair : _components)
        {
            for (Component * component : componentPair.second)
            {
                component->independentUpdate(elapsedTime);
            }
        }
    }

    void GameObject::forEachComponent(std::function <bool(Component *)> func)
    {
        for (auto & componentPair : _components)
        {
            for (Component * component : componentPair.second)
            {
                if (func(component))
                {
                    break;
                }
            }
        }
    }

    void GameObject::forEachComponent(std::function <bool(Component *)> func) const
    {
        for (auto & componentPair : _components)
        {
            for (Component * component : componentPair.second)
            {
                if (func(component))
                {
                    break;
                }
            }
        }
    }

    void GameObject::broadcastMessage(gameplay::AIMessage * message)
    {
        GameObjectController::getInstance().broadcastGameObjectMessage(message, this);
    }

    GameObject * GameObject::getParent() const
    {
        GameObject * parentGameObject = nullptr;
        gameplay::Node * parentNode = _node->getParent();

        if(parentNode)
        {
            parentGameObject = getGameObject(parentNode);
        }

        return parentGameObject;
    }

    gameplay::Node * GameObject::getNode()
    {
        return _node;
    }

    void GameObject::broadcastMessage(gameplay::AIMessage * message, gameplay::Node * broadcastNode)
    {
        if (GameObject * gameObject = getGameObject(broadcastNode))
        {
            gameObject->onMessageReceived(message);
        }
        
        gameplay::Node * child = broadcastNode->getFirstChild();

        while (child != nullptr)
        {
            broadcastMessage(message, child);
            child = child->getNextSibling();
        }
    }

    void GameObject::addComponent(Component * component)
    {
        _components[component->_typeId].push_back(component);
    }

    std::type_index const & GameObject::getComponentTypeId(Component * component)
    {
        return component->_typeId;
    }

    std::string const & GameObject::getComponentId(Component * component)
    {
        return component->_id;
    }

    int GameObject::getNodeUserDataId() const
    {
        return GAMEOBJECT_NODE_USER_DATA_ID;
    }

    GameObject * GameObject::getGameObject(gameplay::Node * node)
    {
        return getNodeUserData<GameObject>(node, GAMEOBJECT_NODE_USER_DATA_ID);
    }

#ifndef _FINAL
    void GameObject::renderDebug(float elapsedTime, gameplay::Font * font)
    {
        for (auto & componentPair : _components)
        {
            for (Component * component : componentPair.second)
            {
                component->renderDebug(elapsedTime, font);
            }
        }
    }

#endif
}
