# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("ShaderCompiler")


set(GLSLANG_INSTALL_DIR "C:/Program Files/glslang-12.2.0/build/$(pwd)/install")


# 包含子项目。

set(SPIRV-Tools_DIR "C:/Program Files/glslang-12.2.0/build/$(pwd)/install/SPIRV-Tools/cmake")
set(SPIRV-Tools-opt_DIR "C:/Program Files/glslang-12.2.0/build/$(pwd)/install/SPIRV-Tools-opt/cmake")
set(glslang_DIR "C:/Program Files/glslang-12.2.0/build/$(pwd)/install/lib/cmake/glslang")

find_package(SPIRV-Tools REQUIRED)
find_package(SPIRV-Tools-opt REQUIRED)
find_package(glslang REQUIRED)

set(GLSLANGLIBS
    glslang
    HLSL
    OSDependent
    OGLCompiler
    SPIRV)

    link_directories(${PROJECT_NAME} ${GLSLANGLIBS})

add_subdirectory ("ShaderCompiler")

target_link_directories(${PROJECT_NAME} PUBLIC ${GLSLANG_INSTALL_DIR})

#target_include_directories(${PROJECT_NAME} PUBLIC ${glslang_INCLUDE_DIRS})