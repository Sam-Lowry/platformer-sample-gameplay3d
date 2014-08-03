#ifndef LUA_GAMEOBJECTCONTROLLER_H_
#define LUA_GAMEOBJECTCONTROLLER_H_

namespace platformer
{

// Lua bindings for GameObjectController.
int lua_GameObjectController_createGameObject(lua_State* state);
int lua_GameObjectController_destroyGameObject(lua_State* state);
int lua_GameObjectController_static_getInstance(lua_State* state);

void luaRegister_GameObjectController();

}

#endif
