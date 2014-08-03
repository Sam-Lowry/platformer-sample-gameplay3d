rootGameObject = nil
levelGameObject = nil

function initialize()
    if Game.getInstance():getConfig():getBool("debug_enable_tools") then
        Game.getInstance():getScriptController():loadScript("res/lua/tools.lua")
    end

    if not Game.getInstance():getConfig():getBool("debug_run_tools_only") then
        -- Create the root gameobject and attach a level to it
        rootGameObject = GameObjectController.getInstance():createGameObject("root")
        levelGameObject = GameObjectController.getInstance():createGameObject("level_0", rootGameObject)
    else
        Game.getInstance():exit()
    end
end
