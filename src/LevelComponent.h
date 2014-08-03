#ifndef PLATFORMER_LEVEL_COMPONENT_H
#define PLATFORMER_LEVEL_COMPONENT_H

#include "Component.h"

namespace gameplay
{
    class Properties;
}

namespace platformer
{
    /**
     * Creates the player, enemy AI and physics defined in a level file (*.level).
     *
     * Level files are created in [Tiled], exported as JSON and then converted to the
     * gameplay property format using [Json2gp3d]
     *
     * [Tiled]      Download @ http://www.mapeditor.org/download.html
     * [Json2gp3d]  Download @ https://github.com/louis-mclaughlin/json-to-gameplay3d
     *
     * @script{ignore}
    */
    class LevelComponent : public Component
    {
    public:
        static int const EMPTY_TILE = 0;

        explicit LevelComponent();
        ~LevelComponent();

        virtual void initialize() override;
        virtual void finalize() override;
        virtual void onMessageReceived(gameplay::AIMessage * message) override;
        virtual void readProperties(gameplay::Properties & properties) override;
        virtual void update(float) override;

        void reload();
        std::string const & getTexturePath() const;
        int getTileWidth() const;
        int getTileHeight() const;
        int getTile(int x, int y) const;
        int getWidth() const;
        int getHeight() const;
        gameplay::Vector2 const & getPlayerSpawnPosition() const;
    private:
        struct Tile
        {
            int _tileId;
            gameplay::Node * _node;
        };

        LevelComponent(LevelComponent const &);

        void load();
        void unload();

        std::string _level;
        std::string _texturePath;
        int _width;
        int _height;
        int _tileWidth;
        int _tileHeight;
        bool _loadBroadcasted;
        gameplay::Vector2 _playerSpawnPosition;
        std::vector<std::vector<Tile>> _grid;
        gameplay::AIMessage * _loadedMessage;
        gameplay::AIMessage * _unloadedMessage;
        std::vector<GameObject*> _children;
    };
}

#endif
