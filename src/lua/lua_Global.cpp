#include "ScriptController.h"
#include "lua_Global.h"

namespace platformer
{

void luaRegister_lua_Global()
{
    gameplay::ScriptUtil::setGlobalHierarchyPair("Game", "Platformer");
    gameplay::ScriptUtil::setGlobalHierarchyPair("PhysicsCollisionObject::CollisionListener", "CollisionHandlerComponent");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "Component");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "GameObject");
    gameplay::ScriptUtil::setGlobalHierarchyPair("Ref", "SpriteSheet");
}

}
