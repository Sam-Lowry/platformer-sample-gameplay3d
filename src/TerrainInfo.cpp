#include "TerrainInfo.h"

#include "NodeData.h"

namespace platformer
{
    TerrainInfo * TerrainInfo::getTerrainInfo(gameplay::Node * node)
    {
        return getNodeUserData<TerrainInfo>(node, NodeUserDataTypes::TerrainInfo);
    }

    int TerrainInfo::getNodeUserDataId() const
    {
        return NodeUserDataTypes::TerrainInfo;
    }
}
