#include "LevelComponent.h"

#include "Common.h"
#include "CollisionObjectComponent.h"
#include "GameObject.h"
#include "GameObjectController.h"
#include "Messages.h"
#include "MessagesInput.h"
#include "MessagesLevel.h"
#include "TerrainInfo.h"

namespace platformer
{
    LevelComponent::LevelComponent()
        : _loadedMessage(nullptr)
        , _unloadedMessage(nullptr)
        , _loadBroadcasted(false)
    {
    }

    LevelComponent::~LevelComponent()
    {
    }

    void LevelComponent::onMessageReceived(gameplay::AIMessage * message)
    {
#ifndef _FINAL
        // Reload on F5 pressed so we can iterate upon it at runtime
        switch (message->getId())
        {
        case Messages::Type::Key:
            {
                KeyMessage keyMessage(message);
                if(keyMessage._event == gameplay::Keyboard::KeyEvent::KEY_PRESS && keyMessage._key == gameplay::Keyboard::Key::KEY_F5)
                {
                    reload();
                }
            }
            break;
        case Messages::Type::RequestLevelReloadMessage:
            reload();
            break;
        }
#endif
    }

    void LevelComponent::reload()
    {
        _loadBroadcasted = false;
    }

    void LevelComponent::update(float)
    {
        if(!_loadBroadcasted)
        {
            unload();
            getRootParent()->broadcastMessage(_unloadedMessage);
            load();
            getRootParent()->broadcastMessage(_loadedMessage);
            _loadBroadcasted = true;
        }
    }

    void LevelComponent::initialize()
    {
        _loadedMessage = LevelLoadedMessage::create();
        _unloadedMessage = LevelUnloadedMessage::create();
    }

    void LevelComponent::finalize()
    {
        unload();
        PLATFORMER_SAFE_DELETE_AI_MESSAGE(_loadedMessage);
        PLATFORMER_SAFE_DELETE_AI_MESSAGE(_unloadedMessage);
    }

    void LevelComponent::load()
    {
        gameplay::Properties * root = createProperties(_level.c_str());
        _width = root->getInt("width");
        _height = root->getInt("height");
        _tileWidth = root->getInt("tilewidth");
        _tileHeight = root->getInt("tileheight");

        int x = 0;
        int y = 0;

        gameplay::Properties * tileCollisionProperties = createProperties("res/physics/level.physics");
        gameplay::Properties * tileCollObjProperties = tileCollisionProperties->getNamespace("tile");
        gameplay::Properties * halfTileCollObjProperties = tileCollisionProperties->getNamespace("half_tile");
        gameplay::Properties * surfaceCollObjProperties = tileCollisionProperties->getNamespace("slope_45");
        gameplay::Properties * ladderCollObjProperties = tileCollisionProperties->getNamespace("ladder");
        gameplay::Properties * resetCollObjProperties = tileCollisionProperties->getNamespace("reset");
        std::map<int, TileType::Enum> tileIdTypes;

        if (gameplay::Properties * tilesetNamespace = root->getNamespace("tilesets_0", true))
        {
            if (gameplay::Properties * tilesetPropertiesNamespace = tilesetNamespace->getNamespace("tileproperties", true))
            {
                while (gameplay::Properties * tileTypeNamespace = tilesetPropertiesNamespace->getNextNamespace())
                {
                    int const tileId = atoi(tileTypeNamespace->getNamespace()) + 1;
                    TileType::Enum tileType = TileType::NONE;
                    char const * tileTypeName = tileTypeNamespace->getString("tile");

                    if(strcmp(tileTypeName, "BOX") == 0)
                    {
                        tileType = TileType::BOX;
                    }
                    else if(strcmp(tileTypeName, "SLOPE_45_L") == 0)
                    {
                        tileType = TileType::SLOPE_45_L;
                    }
                    else if(strcmp(tileTypeName, "SLOPE_45_R") == 0)
                    {
                        tileType = TileType::SLOPE_45_R;
                    }
                    else if(strcmp(tileTypeName, "LADDER") == 0)
                    {
                        tileType = TileType::LADDER;
                    }
                    else if(strcmp(tileTypeName, "RECTANGLE_TOP") == 0)
                    {
                        tileType = TileType::RECTANGLE_TOP;
                    }
                    else if (strcmp(tileTypeName, "RECTANGLE_BOTTOM") == 0)
                    {
                        tileType = TileType::RECTANGLE_BOTTOM;
                    }
                    else if (strcmp(tileTypeName, "RESET") == 0)
                    {
                        tileType = TileType::RESET;
                    }

                    bool const isValidTileType = tileType != TileType::NONE;

                    PLATFORMER_ASSERT(isValidTileType, "Invalid tile type defined for tile %d : '%s'", tileId, tileTypeName);

                    if(isValidTileType)
                    {
                        tileIdTypes[tileId] = tileType;
                    }
                }
            }
        }

        if (gameplay::Properties * layerNamespace = root->getNamespace("layers_0", true))
        {
            if (gameplay::Properties * dataNamespace = layerNamespace->getNextNamespace())
            {
                while (dataNamespace->getNextProperty())
                {
                    if (x == 0)
                    {
                        _grid.push_back(std::vector<Tile>());
                    }

                    int data = dataNamespace->getInt();
                    gameplay::Node * tileNode = nullptr;

                    if (data != EMPTY_TILE)
                    {
                        tileNode = gameplay::Node::create();
                        float const tileWidthScaled = _tileWidth * PLATFORMER_UNIT_SCALAR;
                        float const tileHeightScaled = _tileHeight * PLATFORMER_UNIT_SCALAR;
                        float const tilePosX = ((x * tileWidthScaled) + (tileWidthScaled / 2));
                        float const tilePosY = (-((y * tileHeightScaled) + (tileHeightScaled / 2)));
                        tileNode->translate(tilePosX, tilePosY, 0);
                        getParent()->getNode()->addChild(tileNode);

                        TileType::Enum tileType = TileType::NONE;
                        auto tileIdTypeItr = tileIdTypes.find(data);

                        if(tileIdTypeItr != tileIdTypes.end())
                        {
                            tileType = tileIdTypeItr->second;

                            switch(tileType)
                            {
                            case TileType::BOX:
                                tileNode->setCollisionObject(tileCollObjProperties);
                                break;
                            case TileType::SLOPE_45_L:
                                tileNode->rotateZ(MATH_DEG_TO_RAD(45));
                                tileNode->setCollisionObject(surfaceCollObjProperties);
                                break;
                            case TileType::SLOPE_45_R:
                                tileNode->rotateZ(MATH_DEG_TO_RAD(-45));
                                tileNode->setCollisionObject(surfaceCollObjProperties);
                                break;
                            case TileType::LADDER:
                                tileNode->translateY(tileHeightScaled / 2.0f);
                                tileNode->setCollisionObject(ladderCollObjProperties);
                                break;
                            case TileType::RECTANGLE_TOP:
                                tileNode->translateY(tileHeightScaled / 4.0f);
                                tileNode->setCollisionObject(halfTileCollObjProperties);
                                break;
                            case TileType::RECTANGLE_BOTTOM:
                                tileNode->translateY(-(tileHeightScaled / 4.0f));
                                tileNode->setCollisionObject(halfTileCollObjProperties);
                                break;
                            case TileType::RESET:
                                tileNode->setCollisionObject(resetCollObjProperties);
                                break;
                            default:
                                PLATFORMER_ASSERTFAIL("Unhandled tile type '%d'", tileType);
                            }
                        }

                        TerrainInfo * info = new TerrainInfo();
                        info->_tileType = tileType;
                        tileNode->setUserPointer(info);
                    }

                    _grid[y].emplace_back(Tile{ data, tileNode });
                    ++x;

                    if (x == _width)
                    {
                        x = 0;
                        ++y;

                        if (y == _height)
                        {
                            break;
                        }
                    }
                }
            }
        }

        if (gameplay::Properties * layerNamespace = root->getNamespace("layers_1", true))
        {
            if (gameplay::Properties * objectsNamespace = layerNamespace->getNextNamespace())
            {
                while (gameplay::Properties * objectNamespace = objectsNamespace->getNextNamespace())
                {
                    char const * gameObjectTypeName = objectNamespace->getString("name");
                    GameObject * gameObject = GameObjectController::getInstance().createGameObject(gameObjectTypeName, getParent());
                    gameplay::Vector2 spawnPos(objectNamespace->getInt("x"), -objectNamespace->getInt("y"));
                    spawnPos *= PLATFORMER_UNIT_SCALAR;

                    if(strcmp(gameObjectTypeName, "player") == 0)
                    {
                        _playerSpawnPosition = spawnPos;
                    }

                    std::vector<CollisionObjectComponent*> collisionComponents;
                    gameObject->getComponents(collisionComponents);

                    for(CollisionObjectComponent * collisionComponent : collisionComponents)
                    {
                        if(gameplay::PhysicsCharacter * character = collisionComponent->getNode()->getCollisionObject()->asCharacter())
                        {
                            if(character->isPhysicsEnabled())
                            {
                                collisionComponent->getNode()->setTranslation(spawnPos.x, spawnPos.y, 0);
                            }
                        }
                    }

                    _children.push_back(gameObject);
                }
            }
        }

        if (gameplay::Properties * propertiesNamespace = root->getNamespace("properties", true, false))
        {
            _texturePath = propertiesNamespace->getString("texture");
        }

        SAFE_DELETE(root);
        SAFE_DELETE(tileCollisionProperties);
    }

    void LevelComponent::unload()
    {
        if(!_grid.empty())
        {
            for (int x = 0; x < getWidth(); ++x)
            {
                for (int y = 0; y < getHeight(); ++y)
                {
                    if(gameplay::Node * tileNode = _grid[y][x]._node)
                    {
                        TerrainInfo * info = TerrainInfo::getTerrainInfo(tileNode);
                        tileNode->setUserPointer(nullptr);
                        delete info;
                        getParent()->getNode()->removeChild(tileNode);
                        SAFE_RELEASE(tileNode);
                    }
                }
            }
        }

        _grid.clear();

        for(auto childItr = _children.begin(); childItr != _children.end(); ++childItr)
        {
            GameObjectController::getInstance().destroyGameObject(*childItr);
        }

        _children.clear();
    }

    void LevelComponent::readProperties(gameplay::Properties & properties)
    {
        _level = properties.getString("level", _level.c_str());
        PLATFORMER_ASSERT(gameplay::FileSystem::fileExists(_level.c_str()), "Level '%s' not found", _level.c_str());
    }

    std::string const & LevelComponent::getTexturePath() const
    {
        return _texturePath;
    }

    int LevelComponent::getTileWidth() const
    {
        return _tileWidth;
    }

    int LevelComponent::getTileHeight() const
    {
        return _tileHeight;
    }

    int LevelComponent::getTile(int x, int y) const
    {
        return _grid[y][x]._tileId;
    }

    int LevelComponent::getWidth() const
    {
        return _width;
    }

    int LevelComponent::getHeight() const
    {
        return _height;
    }

    gameplay::Vector2 const & LevelComponent::getPlayerSpawnPosition() const
    {
        return _playerSpawnPosition;
    }
}
