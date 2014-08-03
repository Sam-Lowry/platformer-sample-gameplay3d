#ifndef PLATFORMER__NODE_USER_DATA_TYPES_H
#define PLATFORMER__NODE_USER_DATA_TYPES_H

#include "GameObject.h"

namespace platformer
{
    /**
     * See INodeUserData
     *
     * @script{ignore}
    */
    struct NodeUserDataTypes
    {
        enum Enum
        {
            TerrainInfo = GameObject::GAMEOBJECT_NODE_USER_DATA_ID + 1
        };
    };
}

#endif
