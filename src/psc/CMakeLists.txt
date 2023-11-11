


option(${PROJECT_NAME}_BUILD_EXECUTABLE "enable executable mode" OFF)

set(PSC_LIST "ShaderCompiler.cpp" "GlslangCompilerImpl.h" "DxcCompilerImpl.h")

if(${PROJECT_NAME}_BUILD_EXECUTABLE)
add_executable (${PROJECT_NAME} ${PSC_LIST})
else()
add_library(${PROJECT_NAME} ${PSC_LIST})
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE API_EXPORTS)
