#ifndef LUA_GAMEOBJECT_H_
#define LUA_GAMEOBJECT_H_

namespace platformer
{

// Lua bindings for GameObject.
int lua_GameObject__gc(lua_State* state);
int lua_GameObject__init(lua_State* state);
int lua_GameObject_addRef(lua_State* state);
int lua_GameObject_getRefCount(lua_State* state);
int lua_GameObject_release(lua_State* state);

void luaRegister_GameObject();

}

#endif
