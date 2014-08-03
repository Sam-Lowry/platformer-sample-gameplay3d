#include "Base.h"
#include "ScriptController.h"
#include "lua_GameObjectController.h"
#include "Component.h"
#include "GameObjectController.h"

namespace platformer
{

using namespace gameplay;

void luaRegister_GameObjectController()
{
    const luaL_Reg lua_members[] = 
    {
        {"createGameObject", lua_GameObjectController_createGameObject},
        {"destroyGameObject", lua_GameObjectController_destroyGameObject},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"getInstance", lua_GameObjectController_static_getInstance},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("GameObjectController", lua_members, NULL, NULL, lua_statics, scopePath);
}

static GameObjectController* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "GameObjectController");
    luaL_argcheck(state, userdata != NULL, 1, "'GameObjectController' expected.");
    return (GameObjectController*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_GameObjectController_createGameObject(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    std::string param1 = gameplay::ScriptUtil::getString(2, true);

                    GameObjectController* instance = getInstance(state);
                    void* returnPtr = (void*)instance->createGameObject(param1);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = false;
                        luaL_getmetatable(state, "GameObject");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_GameObjectController_createGameObject - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    std::string param1 = gameplay::ScriptUtil::getString(2, true);

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Node> param2 = gameplay::ScriptUtil::getObjectPointer<Node>(3, "Node", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    GameObjectController* instance = getInstance(state);
                    void* returnPtr = (void*)instance->createGameObject(param1, param2);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = false;
                        luaL_getmetatable(state, "GameObject");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    std::string param1 = gameplay::ScriptUtil::getString(2, true);

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<GameObject> param2 = gameplay::ScriptUtil::getObjectPointer<GameObject>(3, "GameObject", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    GameObjectController* instance = getInstance(state);
                    void* returnPtr = (void*)instance->createGameObject(param1, param2);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = false;
                        luaL_getmetatable(state, "GameObject");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_GameObjectController_createGameObject - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_GameObjectController_destroyGameObject(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<GameObject> param1 = gameplay::ScriptUtil::getObjectPointer<GameObject>(2, "GameObject", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'GameObject'.");
                    lua_error(state);
                }

                GameObjectController* instance = getInstance(state);
                instance->destroyGameObject(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_GameObjectController_destroyGameObject - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_GameObjectController_static_getInstance(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            void* returnPtr = (void*)&(GameObjectController::getInstance());
            if (returnPtr)
            {
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                object->instance = returnPtr;
                object->owns = false;
                luaL_getmetatable(state, "GameObjectController");
                lua_setmetatable(state, -2);
            }
            else
            {
                lua_pushnil(state);
            }

            return 1;
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 0).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
