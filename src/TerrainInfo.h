#ifndef PLATFORMER_TERRAIN_INFO_H
#define PLATFORMER_TERRAIN_INFO_H

#include "Common.h"

namespace platformer
{
    /**
     * The unique tile types that can be set per tile in a level.
     *
     * @script{ignore}
    */
    struct TileType
    {
        enum Enum
        {
            NONE,
            BOX,
            RECTANGLE_TOP,
            RECTANGLE_BOTTOM,
            SLOPE_45_L,
            SLOPE_45_R,
            LADDER,
            RESET
        };
    };

    /** @script{ignore} */
    class TerrainInfo : public INodeUserData
    {
    public:
        static TerrainInfo * getTerrainInfo(gameplay::Node * node);

        int getNodeUserDataId() const override;

        TileType::Enum _tileType;
    };
}

#endif
