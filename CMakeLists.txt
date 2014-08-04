cmake_minimum_required(VERSION 2.7)
PROJECT(platformer)

set(GAME_NAME platformer)

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set(ARCH_DIR "x64" )
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set(ARCH_DIR "x86" )
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

set(GAMEPLAY_SRC_PATH "../GamePlay") 
set(GAMEPLAY_EXT_LIBS_PATH "${GAMEPLAY_SRC_PATH}/external-deps")

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    ADD_DEFINITIONS(-D__linux__)
    SET(TARGET_OS "LINUX")
    SET(TARGET_OS_DIR "linux")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    IF(MSVC)
        ADD_DEFINITIONS(-DMSVC)
    ENDIF(MSVC)
    ADD_DEFINITIONS(-DWIN32)
    ADD_DEFINITIONS(-D_WINDOWS)
    SET(TARGET_OS "WINDOWS")
    SET(TARGET_OS_DIR "windows")
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Linux")

set(GAME_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/${TARGET_OS_DIR}")

macro (append_gameplay_lib listToAppend)
    set(libName gameplay)
    IF (TARGET_OS STREQUAL "WINDOWS")
        FIND_LIBRARY(${libName}_LIBRARY_RELEASE
            NAMES ${libName}
            PATHS "${GAMEPLAY_SRC_PATH}/gameplay/${TARGET_OS_DIR}/${ARCH_DIR}/Release"
        )

        FIND_LIBRARY(${libName}_LIBRARY_DEBUG
            NAMES ${libName}
            PATHS "${GAMEPLAY_SRC_PATH}/gameplay/${TARGET_OS_DIR}/${ARCH_DIR}/Debug"
        )
        SET(FOUND_LIB_${libName}
            debug ${${libName}_LIBRARY_DEBUG}
            optimized ${${libName}_LIBRARY_RELEASE}
        )
    ELSE (TARGET_OS STREQUAL "WINDOWS")
        find_library(FOUND_LIB_${libName} ${libName} HINTS
            "${GAMEPLAY_SRC_PATH}/cmake/gameplay" "${GAMEPLAY_SRC_PATH}/build/gameplay" "${GAMEPLAY_SRC_PATH}/gameplay/src")
    ENDIF (TARGET_OS STREQUAL "WINDOWS")
    set(${listToAppend} ${${listToAppend}} ${FOUND_LIB_${libName}})
endmacro(append_gameplay_lib)

macro (append_gameplay_ext_lib listToAppend libName libDirName)
    IF("${libDirName}" STREQUAL "")
        find_library(FOUND_LIB_${libName} NAMES ${libName} ${ARGN})
    ELSE("${libDirName}" STREQUAL "")
        set(pathToSearch
            "${GAMEPLAY_EXT_LIBS_PATH}/${libDirName}/lib/${TARGET_OS_DIR}/${ARCH_DIR}")
        find_library(FOUND_LIB_${libName} NAMES ${libName} ${ARGN} HINTS ${pathToSearch})
    ENDIF("${libDirName}" STREQUAL "")

    set(${listToAppend} ${${listToAppend}} ${FOUND_LIB_${libName}})
    message(STATUS "Library Found: ${libName} Path: ${FOUND_LIB_${libName}}")
endmacro (append_gameplay_ext_lib)


macro(copy_files TARGET_NAME GLOBPAT SOURCE DESTINATION RECUR)
    get_filename_component(REALPATH_SOURCE ${SOURCE} REALPATH)
    IF(${RECUR})
        SET(RECURSE_PARAM GLOB_RECURSE)
    ELSEIF(NOT ${RECUR})
        SET(RECURSE_PARAM GLOB)
    ENDIF(${RECUR})
    file(${RECURSE_PARAM} COPY_FILES RELATIVE ${REALPATH_SOURCE} "${REALPATH_SOURCE}/${GLOBPAT}")

  add_custom_target(${TARGET_NAME} ALL COMMENT "Copying files: ${REALPATH_SOURCE}/${GLOBPAT}")

  foreach(FILENAME ${COPY_FILES})
    set(SRC "${REALPATH_SOURCE}/${FILENAME}")
    set(DST "${DESTINATION}/${FILENAME}")

    IF(IS_DIRECTORY ${SRC})
        add_custom_command(TARGET ${TARGET_NAME} COMMAND ${CMAKE_COMMAND} -E make_directory ${DST})
    ELSE(IS_DIRECTORY ${SRC})
        add_custom_command(TARGET ${TARGET_NAME} COMMAND ${CMAKE_COMMAND} -E copy ${SRC} ${DST})
    ENDIF(IS_DIRECTORY ${SRC})
  endforeach(FILENAME)
endmacro(copy_files)

macro(add_source_group SOURCE_PATH SOURCES)
    if(WIN32)
        STRING(REPLACE "/" "\\\\" SOURCE_GROUP_PATH ${SOURCE_PATH})
    endif()
    source_group(${SOURCE_GROUP_PATH} FILES ${SOURCES})
endmacro(add_source_group)

include_directories( 
    ${GAMEPLAY_SRC_PATH}/gameplay/src
    ${CMAKE_SOURCE_DIR}/src
    ${GAMEPLAY_SRC_PATH}/external-deps/lua/include
    ${GAMEPLAY_SRC_PATH}/external-deps/bullet/include
    ${GAMEPLAY_SRC_PATH}/external-deps/png/include
    ${GAMEPLAY_SRC_PATH}/external-deps/oggvorbis/include
    ${GAMEPLAY_SRC_PATH}/external-deps/zlib/include
    ${GAMEPLAY_SRC_PATH}/external-deps/openal/include
    ${GAMEPLAY_SRC_PATH}/external-deps/openal/include/AL
    ${GAMEPLAY_SRC_PATH}/external-deps/glew/include
)

append_gameplay_lib(GAMEPLAY_LIBRARIES)
append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "GLEW" "glew" "glew32")
append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "lua" "lua")
append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "png" "png" "libpng")
append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "BulletDynamics" "bullet")
append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "BulletCollision" "bullet")
append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "LinearMath" "bullet")
append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "openal" "openal" "openal32")
append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "vorbis" "oggvorbis" "libvorbis")
append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "ogg" "oggvorbis" "libogg")
append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "z" "zlib" "zlib")

IF (TARGET_OS STREQUAL "LINUX")
    append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "GL" "")
    append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "m" "" )
    append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "X11" "")
    append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "dl" "")
    append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "rt" "" )
    append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "pthread" "" )
    append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "gtk-x11-2.0" "" )
    append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "gobject-2.0" "" )
    append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "glib-2.0" "" )

ELSEIF (TARGET_OS STREQUAL "WINDOWS")
    set(GAMEPLAY_LIBRARIES ${GAMEPLAY_LIBRARIES} "OpenGL32")
    set(GAMEPLAY_LIBRARIES ${GAMEPLAY_LIBRARIES} "GLU32")
    append_gameplay_ext_lib(GAMEPLAY_LIBRARIES "vorbisfile" "oggvorbis" "libvorbisfile")
    ADD_DEFINITIONS(-D_ITERATOR_DEBUG_LEVEL=0)
ENDIF (TARGET_OS STREQUAL "LINUX")

# This file was generated by newproject.sh but it's been modified from this point onwards:

# CMake recommends that you generate projects that are 'out of source' meaning that you create a 'build'
# folder in the top level directory that doesn't pollute the original src directory with project/CMake specific data.
# This is a good idea but it means the working directory of the game while debugging is now './build' instead of
# './' so the game won't be able to find any of the resources. The game shouldn't have to be aware of this nor do
# we want to have to copy any resource changes to the build folder everytime we change something. We can get around
# this by creating symlinks in the build folder that point the actual resources in the parent folder.
if(WIN32)
    if(NOT EXISTS "game.config")
        execute_process(COMMAND cmd /C ${CMAKE_SOURCE_DIR}/mklink_win32.bat)
    endif()
else()
    if(NOT EXISTS res)
        execute_process(COMMAND ln -s../res res)
    endif()
    if(NOT EXISTS game.config)
        execute_process(COMMAND ln -s ../game.config game.config)
    endif()
endif()

# A listing of all the project files
set(GAME_CONFIG ./build/game.config)
set(ANDROID ./android/AndroidManifest.xml ./android/build.xml ./android/res/values/strings.xml ./android/jni/Android.mk ./android/jni/Application.mk)
file(GLOB RAW_LEVELS ./raw/levels/*)
file(GLOB RAW_SPRITESHEETS ./raw/spritesheets/*)
file(GLOB RAW_TEMPLATES ./raw/templates/*)
file(GLOB RES_AUDIO ./res/audio/*.audio)
file(GLOB RES_LUA ./res/lua/*.lua)
file(GLOB RES_LUA_TOOLS ./res/lua/tools/*)
file(GLOB RES_GAME_OBJECTS ./res/gameobjects/*)
file(GLOB RES_LEVELS ./res/levels/*)
file(GLOB RES_PHYSICS ./res/physics/*)
file(GLOB RES_SCENES ./res/scenes/*)
file(GLOB RES_SHADERS ./res/shaders/*)
file(GLOB RES_SPRITESHEETS ./res/spritesheets/*)
file(GLOB RES_UI ./res/ui/*)
file(GLOB GAME_SRC ./src/*.cpp ./src/*.h ./src/*.inl)
file(GLOB GAME_SRC_LUA ./src/lua/*.cpp ./src/lua/*.h)

# How the project files will be presented in an IDE, not all of them support this, some will just
# display the file structure as it appears on disk
set(SG_ANDROID "android")
set(SG_GAME_SRC "src")
set(SG_GAME_SRC_LUA "src/lua")
set(SG_GAME_CONFIG "res")
set(SG_RAW_LEVELS "raw/levels")
set(SG_RAW_TEMPLATES "raw/templates")
set(SG_RAW_SPRITESHEETS "raw/spritesheets")
set(SG_RES_AUDIO "res/audio")
set(SG_RES_LUA "res/lua")
set(SG_RES_LUA_TOOLS "res/lua/tools")
set(SG_RES_GAME_OBJECTS "res/gameobjects")
set(SG_RES_LEVELS "res/levels")
set(SG_RES_PHYSICS "res/physics")
set(SG_RES_SCENES "res/scenes")
set(SG_RES_SHADERS "res/shaders")
set(SG_RES_SPRITESHEETS "res/spritesheets")
set(SG_RES_UI "res/ui")

if(WIN32)

    # The behaviour for passing file list variables as macro arguments seems to differ
    # on Windows. The list gets truncated to include only its first entry and is converted
    # to a string without being expanded e.g. './res/*.txt' . Converting a list to a string
    # before passing it as an argument prevents truncation causes it to get fully expanded.

    add_source_group(${SG_ANDROID} "${ANDROID}")
    add_source_group(${SG_GAME_SRC} "${GAME_SRC}")
    add_source_group(${SG_GAME_SRC_LUA} "${GAME_SRC_LUA}")
    add_source_group(${SG_GAME_CONFIG} "${GAME_CONFIG}")
    add_source_group(${SG_RAW_LEVELS} "${RAW_LEVELS}")
    add_source_group(${SG_RAW_TEMPLATES} "${RAW_TEMPLATES}")
    add_source_group(${SG_RAW_SPRITESHEETS} "${RAW_SPRITESHEETS}")
    add_source_group(${SG_RES_AUDIO} "${RES_AUDIO}")
    add_source_group(${SG_RES_LUA} "${RES_LUA}")
    add_source_group(${SG_RES_LUA_TOOLS} "${RES_LUA_TOOLS}")
    add_source_group(${SG_RES_GAME_OBJECTS} "${RES_GAME_OBJECTS}")
    add_source_group(${SG_RES_LEVELS} "${RES_LEVELS}")
    add_source_group(${SG_RES_PHYSICS} "${RES_PHYSICS}")
    add_source_group(${SG_RES_SCENES} "${RES_SCENES}")
    add_source_group(${SG_RES_SPRITESHEETS} "${RES_SPRITESHEETS}")
    add_source_group(${SG_RES_SHADERS} "${RES_SHADERS}")
    add_source_group(${SG_RES_UI} "${RES_UI}")
else()
    add_source_group(${SG_ANDROID} ${ANDROID})
    add_source_group(${SG_GAME_SRC} ${GAME_SRC})
    add_source_group(${SG_GAME_SRC_LUA} ${GAME_SRC_LUA})
    add_source_group(${SG_GAME_CONFIG} ${GAME_CONFIG})
    add_source_group(${SG_RAW_LEVELS} ${RAW_LEVELS})
    add_source_group(${SG_RAW_TEMPLATES} ${RAW_TEMPLATES})
    add_source_group(${SG_RAW_SPRITESHEETS} ${RAW_SPRITESHEETS})
    add_source_group(${SG_RES_AUDIO} ${RES_AUDIO})
    add_source_group(${SG_RES_LUA} ${RES_LUA})
    add_source_group(${SG_RES_LUA_TOOLS} ${RES_LUA_TOOLS})
    add_source_group(${SG_RES_GAME_OBJECTS} ${RES_GAME_OBJECTS})
    add_source_group(${SG_RES_LEVELS} ${RES_LEVELS})
    add_source_group(${SG_RES_PHYSICS} ${RES_PHYSICS})
    add_source_group(${SG_RES_SCENES} ${RES_SCENES})
    add_source_group(${SG_RES_SPRITESHEETS} ${RES_SPRITESHEETS})
    add_source_group(${SG_RES_SHADERS} ${RES_SHADERS})
    add_source_group(${SG_RES_UI} ${RES_UI})
endif()

add_executable(${GAME_NAME}
    WIN32
    ${GAME_SRC}
    ${GAME_SRC_LUA}
    ${ANDROID}
    ${GAME_CONFIG}
    ${RES_GAME_OBJECTS}
    ${RES_SPRITESHEETS}
    ${RES_SCENES}
    ${RAW_SPRITESHEETS}
    ${RAW_LEVELS}
    ${RES_AUDIO}
    ${RES_LEVELS}
    ${RES_PHYSICS}
    ${RES_LUA}
    ${RES_LUA_TOOLS}
    ${RAW_TEMPLATES}
    ${RES_UI}
    ${RES_SHADERS}
)

target_link_libraries(${GAME_NAME} ${GAMEPLAY_LIBRARIES})

set_target_properties(${GAME_NAME} PROPERTIES
    OUTPUT_NAME "${GAME_NAME}"
    RUNTIME_OUTPUT_DIRECTORY "${GAME_OUTPUT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${GAME_OUTPUT_DIR}"
)

if ( "${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG" )
    # add_definitions(-DGP_USE_MEM_LEAK_DETECTION)
    add_definitions(-D_DEBUG)
endif()

if(MSVC)
    # Enable multi-processor compilation for faster compile times
    add_definitions(/MP)
else()
    # Enable C++11
    set(CMAKE_CXX_FLAGS "-std=c++11")
    add_definitions(-lstdc++)
endif()
