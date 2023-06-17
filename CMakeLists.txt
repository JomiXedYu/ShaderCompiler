# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("ShaderCompiler")
set(CMAKE_CXX_STANDARD 23)


cmake_policy(SET CMP0079 NEW)


add_executable (ShaderCompiler "ShaderCompiler.cpp" "ShaderCompiler.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ShaderCompiler PROPERTY CXX_STANDARD 20)
endif()

set(GLSLANGLIBS 
    GenericCodeGend
    MachineIndependentd
    glslangd
    HLSLd
    OSDependentd
    OGLCompilerd
    SPIRVd
    SPIRV-Toolsd
    SPIRV-Tools-optd
)

target_include_directories(${PROJECT_NAME} PRIVATE "./lib")
target_link_directories(${PROJECT_NAME} PRIVATE "./lib/Debug")
target_link_libraries(${PROJECT_NAME} "${GLSLANGLIBS}")

#target_include_directories(${PROJECT_NAME} PUBLIC ${glslang_INCLUDE_DIRS})