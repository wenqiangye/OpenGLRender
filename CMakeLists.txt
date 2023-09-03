cmake_minimum_required(VERSION 3.16)

project(openglrender)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/src/header
)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

if(APPLE)
    include_directories(
        /usr/local/include
        /opt/local/include
        /opt/homebrew/include
    )
endif()
find_package(OpenGL REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)

if(NOT Boost_FOUND)
    message("Not Found Boost")
endif()
file(GLOB SRC 
    ./imgui/*.h
    ./imgui/*.cpp  
    ./src/header/*.h
    ./src/header/*.cpp  
    ./src/header/glad.c
)

if(WIN32)
    file(GLOB PLATFORM_SRC
        ./imgui/backends/imgui_impl_win32.*
        ./imgui/backends/imgui_impl_dx12.*
        ./src/win/main.cpp
    )
elseif(UNIX)
    add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GL3W)

    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/examples/libs/gl3w
    )

    file(GLOB PLATFORM_SRC
        ./imgui/examples/libs/gl3w/GL/gl3w.*
        ./imgui/backends/imgui_impl_glfw.*
        ./imgui/backends/imgui_impl_opengl3.*
        ./src/unix/main.cpp
        ./src/control/*.h
        ./src/control/*.cpp

    )
endif()

set(EXEC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${EXEC_PATH})
add_executable(${PROJECT_NAME} ${SRC} ${PLATFORM_SRC})

if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        d3d12.lib
        d3dcompiler.lib
        dxgi.lib
    )
elseif(APPLE)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVID_LIBRARY CoreVideo REQUIRED)
    message(${COCOA_LIBRARY})
    message(${IOKIT_LIBRARY})
    message(${COREVID_LIBRARY})

    target_link_libraries(${PROJECT_NAME}
        ${OPENGL_LIBRARY}
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVID_LIBRARY}
        glfw # use this lib name
    )
elseif (UNIX AND NOT APPLE)

    # linux: sudo apt install libglfw3-dev
    target_link_libraries(${PROJECT_NAME}
        ${Boost_LIBRARIES}
        GL
        glfw # use this lib name
        dl
        assimp
    )
endif()


