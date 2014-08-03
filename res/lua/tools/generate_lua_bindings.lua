mkdir(_toolsRoot .. "/src/lua")
rm(_toolsRoot .. "/build/xml")
os.execute("doxygen ../game.doxyfile")
os.execute(_toolsRoot .. "/../GamePlay/bin/" .. Game.getInstance():getConfig():getString("debug_os") ..
            "/gameplay-luagen " .. _toolsRoot .. "/build/xml/ " .. _toolsRoot .. "/src/lua/ platformer")
