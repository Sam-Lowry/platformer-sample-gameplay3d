#include "Base.h"
#include "lua_all_bindings.h"

namespace platformer
{

void lua_RegisterAllBindings()
{
    luaRegister_GameObject();
    luaRegister_GameObjectController();
    luaRegister_lua_Global();
}

}

