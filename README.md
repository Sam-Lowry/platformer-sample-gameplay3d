#2D platformer sample v1.0.0

A 2D platformer made using the open source game framework Gameplay3D, other freely available cross platform tools and free to use assets.

![ScreenShot](https://raw.githubusercontent.com/louis-mclaughlin/platformer-sample-gameplay3d/master/raw/textures/platformer_big.jpg)

### Usage
- Download and build [Gameplay3D](http://www.gameplay3d.org/) for your platform
- Clone this repository into the same directory as the 'GamePlay' directory:

```
├── GamePlay
└── platformer-sample-gameplay3d
```

- Run [CMake](http://www.cmake.org/) and compile the generated project:

```
cd platformer-sample-gameplay3d
mkdir build
cd build
cmake ..
```

- Building the sample for android requires that you build Gameplay3D for android with the following in Application.mk:

```
APP_PLATFORM := android-10
APP_STL := gnustl_static
APP_ABI := armeabi-v7a
APP_CPPFLAGS += -frtti -std=gnu++11
NDK_TOOLCHAIN_VERSION := clang
APP_MODULES := libgameplay
```

- See 'game.config' for configuarable debug options and tools
- Use either a gamepad or the keyboard (directional arrows/WASD + space) to make the player move and jump

### Gameplay3D features used
- Audio sources
- Camera
- Collision listeners
- Gamepad and keyboard input
- Lua binding and scripting
- Physics character/rigid bodies/ghost objects
- Resource aliasing
- Scene graph, nodes and node user data
- Sprite batch and sprite font rendering
- Virtual controls

### Additional features
- Cross platform lua toolchain:
 - Automated Android project generation, building and deployment
 - Automated Lua binding generation
- Data driven entity-component system, includes message passing
- Debug assists
 - Annotated asserts
 - Rendered asserts and logs
 - Rendered game logic information
- Tile map generation
- Sprite animation
- Sym-linked resources for debug builds

### Tools used
- [Audacity](http://audacity.sourceforge.net/)
- [GIMP](http://www.gimp.org/)
- [json-to-gameplay3d](https://github.com/louis-mclaughlin/json-to-gameplay3d)
- [SFXR](http://www.drpetter.se/project_sfxr.html)
- [TexturePacker (lite)](https://www.codeandweb.com/texturepacker)
- [Tiled](http://www.mapeditor.org/)

### Content pipeline
- Audio: SFXR -> (.wav) -> Audacity (.ogg)
- Levels: Tiled (.json) -> json-to-gameplay3d (.level)
- Spritesheets: TexturePacker (.json) -> json-to-gameplay3d (.ss)
- Textures: GIMP (.png)

### External assets used
- [https://www.google.com/fonts/specimen/Open+Sans](https://www.google.com/fonts/specimen/Open+Sans)
- [http://kenney.nl/assets](http://kenney.nl/assets)
- [https://github.com/blackberry/GamePlay/blob/master/gameplay/res/logo_black.png](https://github.com/blackberry/GamePlay/blob/master/gameplay/res/logo_black.png)

### Compatibility

This sample has never been tested on MacOS nor deployed to IOS only because I don't currently have access to either.

This sample makes extensive use of C++11.

####Built with the following compilers:
- MSVC v120
- Clang 3.5
- GCC 4.6.1

####Tested on the following operating systems:
- Android 2.3.5, 4.1.2
- Ubuntu 14.04 LTS
- Windows 7, 8.0

####Tested on the following android devices:
- HTC Desire S
- Samsung Galaxy Note S 10.1
