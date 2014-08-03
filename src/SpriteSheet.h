#ifndef PLATFORMER_SPRITESHEET_H
#define PLATFORMER_SPRITESHEET_H

#include "gameplay.h"

namespace platformer
{
    /** @script{ignore} */
    struct Sprite
    {
        std::string _name;
        gameplay::Rectangle _src;
        bool _isRotated;
    };

    /**
     * Loads a sprite sheet from a file (*.ss)
     *
     * Spritesheets are created in [TexturePacker], exported as JSON and then converted to the
     * gameplay property format using [Json2gp3d]
     *
     * [TexturePacker]  Download @ http://www.codeandweb.com/texturepacker/download
     * [Json2gp3d]      Download @ https://github.com/louis-mclaughlin/json-to-gameplay3d
     *
     * @script{ignore}
    */
    class SpriteSheet : public gameplay::Ref
    {
    public:
        static SpriteSheet * create(std::string const & spriteSheetPath);
        ~SpriteSheet();

        Sprite * getSprite(std::string const & spriteName);
        gameplay::Rectangle const & getSrc();
        gameplay::Texture * getTexture();
        std::string const & getName() const;
    private:
        struct SpriteFactory
        {
            static SpriteFactory & getInstance();
            std::map<std::string, SpriteSheet *> _sprites;
        };

        explicit SpriteSheet();
        SpriteSheet(SpriteSheet const &);

        void initialize(std::string const & filePath);

        gameplay::Rectangle _src;
        gameplay::Texture * _texture;
        std::string _name;
        std::map<std::string, Sprite> _sprites;
    };
}

#endif
