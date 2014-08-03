os.execute("android update project -t 1 -p ../android -s")
os.execute("ndk-build -C ../android -j " ..  Game.getInstance():getConfig():getInt("debug_num_android_compile_jobs"))
