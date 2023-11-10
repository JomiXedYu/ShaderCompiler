#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <filesystem>
#include "BinaryFileHeader.h"
#include "Classes.hpp"

namespace psc
{
    using IncludePaths = std::vector<std::filesystem::path>;

    struct CompileInfo
    {
        bool Debug = false;
        IncludePaths IncludePaths;
        std::vector<std::string> PreDefines;
    };

    class __declspec(dllexport) ShaderCompiler
    {
    public:
        virtual std::vector<char> CompileStage(
            const char* code, 
            PlatformType platform, 
            FilePartialType Stage, 
            const CompileInfo& compileInfo) = 0;

        virtual void CompileShader(
            std::filesystem::path shPath,
            CompileInfo compileInfo,
            const std::vector<PlatformType_>& TargetPlatforms,
            std::ostream& out) = 0;

    };

    extern __declspec(dllexport) std::shared_ptr<ShaderCompiler> CreateShaderCompiler(PlatformType_ platform);
}
