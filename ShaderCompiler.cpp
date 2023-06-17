
#include "ShaderCompiler.h"
#include <glslang/Include/Common.h>
#include <glslang/Include/glslang_c_interface.h>
#include <vector>
#include <fstream>
#include <filesystem>
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <StandAlone/DirStackFileIncluder.h>
#include <stdexcept>

namespace IOHelper
{
    using namespace std;

    inline string ReadAllText(std::string path)
    {
        ifstream fin;
        fin.open(path, std::ios::ate | std::ios::binary);
        if (!fin.is_open())
        {
            cout << "无法找到这个文件！" << endl;
            return {};
        }
        size_t fileSize = (size_t)fin.tellg();
        string buffer(fileSize, ' ');

        fin.seekg(0);

        fin.read(buffer.data(), fileSize);

        fin.close();

        return buffer;
    }

    inline void WriteFile(std::string path, const std::vector<char>& data)
    {
        std::filesystem::path dir(path);
        dir.remove_filename();

        if (!std::filesystem::exists(dir))
        {
            std::filesystem::create_directory(dir);
        }

        ofstream fs(path, ios::out | ios::binary);

        fs.write(data.data(), data.size());
        fs.close();
    }

}

namespace vkshc
{
    using namespace std;

    EShLanguage GetStage(std::string ext)
    {
        static std::unordered_map<std::string, EShLanguage> map
        {
            {"vs", EShLanguage::EShLangVertex },
            {"ps", EShLanguage::EShLangFragment }
        };

        auto it = map.find(ext);
        if (it == map.end())
        {
            assert(false);
        }
        return it->second;
    }

    std::string GetFilePath(const std::string& str)
    {
        size_t found = str.find_last_of("/\\");
        return str.substr(0, found);
    }

    TBuiltInResource GetDefaultTBuiltInResource()
    {
        TBuiltInResource res{};
        res.maxLights = 32;
        res.maxClipPlanes = 6;
        res.maxTextureUnits = 32;
        res.maxTextureCoords = 32;
        res.maxVertexAttribs = 64;
        res.maxVertexUniformComponents = 4096;
        res.maxVaryingFloats = 64;
        res.maxVertexTextureImageUnits = 32;
        res.maxCombinedTextureImageUnits = 80;
        res.maxTextureImageUnits = 32;
        res.maxFragmentUniformComponents = 4096;
        res.maxDrawBuffers = 32;
        res.maxVertexUniformVectors = 128;
        res.maxVaryingVectors = 8;
        res.maxFragmentUniformVectors = 16;
        res.maxVertexOutputVectors = 16;
        res.maxFragmentInputVectors = 15;
        res.minProgramTexelOffset = -8;
        res.maxProgramTexelOffset = 7;
        res.maxClipDistances = 8;
        res.maxComputeWorkGroupCountX = 65535;
        res.maxComputeWorkGroupCountY = 65535;
        res.maxComputeWorkGroupCountZ = 65535;
        res.maxComputeWorkGroupSizeX = 1024;
        res.maxComputeWorkGroupSizeY = 1024;
        res.maxComputeWorkGroupSizeZ = 64;
        res.maxComputeUniformComponents = 1024;
        res.maxComputeTextureImageUnits = 16;
        res.maxComputeImageUniforms = 8;
        res.maxComputeAtomicCounters = 8;
        res.maxComputeAtomicCounterBuffers = 1;
        res.maxVaryingComponents = 60;
        res.maxVertexOutputComponents = 64;
        res.maxGeometryInputComponents = 64;
        res.maxGeometryOutputComponents = 128;
        res.maxFragmentInputComponents = 128;
        res.maxImageUnits = 8;
        res.maxCombinedImageUnitsAndFragmentOutputs = 8;
        res.maxCombinedShaderOutputResources = 8;
        res.maxImageSamples = 0;
        res.maxVertexImageUniforms = 0;
        res.maxTessControlImageUniforms = 0;
        res.maxTessEvaluationImageUniforms = 0;
        res.maxGeometryImageUniforms = 0;
        res.maxFragmentImageUniforms = 8;
        res.maxCombinedImageUniforms = 8;
        res.maxGeometryTextureImageUnits = 16;
        res.maxGeometryOutputVertices = 256;
        res.maxGeometryTotalOutputComponents = 1024;
        res.maxGeometryUniformComponents = 1024;
        res.maxGeometryVaryingComponents = 64;
        res.maxTessControlInputComponents = 128;
        res.maxTessControlOutputComponents = 128;
        res.maxTessControlTextureImageUnits = 16;
        res.maxTessControlUniformComponents = 1024;
        res.maxTessControlTotalOutputComponents = 4096;
        res.maxTessEvaluationInputComponents = 128;
        res.maxTessEvaluationOutputComponents = 128;
        res.maxTessEvaluationTextureImageUnits = 16;
        res.maxTessEvaluationUniformComponents = 1024;
        res.maxTessPatchComponents = 120;
        res.maxPatchVertices = 32;
        res.maxTessGenLevel = 64;
        res.maxViewports = 16;
        res.maxVertexAtomicCounters = 0;
        res.maxTessControlAtomicCounters = 0;
        res.maxTessEvaluationAtomicCounters = 0;
        res.maxGeometryAtomicCounters = 0;
        res.maxFragmentAtomicCounters = 8;
        res.maxCombinedAtomicCounters = 8;
        res.maxAtomicCounterBindings = 1;
        res.maxVertexAtomicCounterBuffers = 0;
        res.maxTessControlAtomicCounterBuffers = 0;
        res.maxTessEvaluationAtomicCounterBuffers = 0;
        res.maxGeometryAtomicCounterBuffers = 0;
        res.maxFragmentAtomicCounterBuffers = 1;
        res.maxCombinedAtomicCounterBuffers = 1;
        res.maxAtomicCounterBufferSize = 16384;
        res.maxTransformFeedbackBuffers = 4;
        res.maxTransformFeedbackInterleavedComponents = 64;
        res.maxCullDistances = 8;
        res.maxCombinedClipAndCullDistances = 8;
        res.maxSamples = 4;

        res.limits.nonInductiveForLoops = 1;
        res.limits.whileLoops = 1;
        res.limits.doWhileLoops = 1;
        res.limits.generalUniformIndexing = 1;
        res.limits.generalAttributeMatrixVectorIndexing = 1;
        res.limits.generalVaryingIndexing = 1;
        res.limits.generalSamplerIndexing = 1;
        res.limits.generalVariableIndexing = 1;
        res.limits.generalConstantMatrixVectorIndexing = 1;

        return res;
    }

    static std::vector<char> _CompileShaderToSPIRV(
        const char* code, 
        EShLanguage type,
        const std::vector<string>& includePaths,
        const CompileInfo& compileInfo = CompileInfo{},
        glslang::EShSource srcLang = glslang::EShSource::EShSourceHlsl
        )
    {
        glslang::TShader shader(type);
        shader.setStrings(&code, 1);

        glslang::EShClient client = glslang::EShClient::EShClientVulkan;

        int ClientInputSemanticsVersion = 100;

        glslang::EShTargetClientVersion VulkanClientVersion = glslang::EShTargetVulkan_1_1;
        glslang::EShTargetLanguageVersion TargetVersion = glslang::EShTargetSpv_1_3;

        shader.setEnvInput(srcLang, type, client, ClientInputSemanticsVersion);
        shader.setEnvClient(client, VulkanClientVersion);
        shader.setEnvTarget(glslang::EShTargetSpv, TargetVersion);

        TBuiltInResource resources{};
        resources = GetDefaultTBuiltInResource();

        EShMessages messages;
        messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

        const int DefaultVersion = 100;

        DirStackFileIncluder includer;

        for (auto& path : includePaths)
        {
            includer.pushExternalLocalDirectory(path);
        }

        std::string PreprocessedGLSL;

        if (!shader.preprocess(&resources, DefaultVersion, ENoProfile, false, false, messages, &PreprocessedGLSL, includer))
        {
            std::cout << shader.getInfoLog() << std::endl;
            std::cout << shader.getInfoDebugLog() << std::endl;
            throw std::runtime_error("Preprocessing Failed");
        }

        const char* PreprocessedCStr = PreprocessedGLSL.c_str();
        shader.setStrings(&PreprocessedCStr, 1);

        if (!shader.parse(&resources, 100, false, messages))
        {
            std::cout << shader.getInfoLog() << std::endl;
            std::cout << shader.getInfoDebugLog() << std::endl;
            throw std::runtime_error("Parsing Failed");
        }

        glslang::TProgram Program;
        Program.addShader(&shader);

        if (!Program.link(messages))
        {
            std::cout << shader.getInfoLog() << std::endl;
            std::cout << shader.getInfoDebugLog() << std::endl;
            throw std::runtime_error("Linking Failed");
        }

        if (!Program.mapIO())
        {
            std::cout << shader.getInfoLog() << std::endl;
            std::cout << shader.getInfoDebugLog() << std::endl;
            throw std::runtime_error("Linking (Mapping IO) Failed");
        }

        std::vector<unsigned int> SpirV;
        spv::SpvBuildLogger logger;
        glslang::SpvOptions spvOptions;

        spvOptions.generateDebugInfo = compileInfo.Debug;

        glslang::GlslangToSpv(*Program.getIntermediate(type), SpirV, &logger, &spvOptions);

        cout << logger.getAllMessages() << endl;

        std::vector<char> spirData(SpirV.size() * sizeof(int));
        memcpy(spirData.data(), SpirV.data(), SpirV.size() * sizeof(int));

        return spirData;
    }

    static void _StreamWriteSize(std::vector<char>& stream, int32_t size)
    {
        char buf[sizeof(int32_t)] = { 0 };
        *((int32_t*)buf) = (int32_t)size;
        stream.insert(stream.end(), buf, buf + sizeof(int32_t));
    }

    std::vector<char> CompileShader(
        const std::vector<string>& includePaths,
        std::string shaderPath,
        const CompileInfo& compileInfo)
    {
        auto vert = shaderPath + ".vs.hlsl";
        auto frag = shaderPath + ".ps.hlsl";
        auto cfg = shaderPath + ".shcfg";

        auto vertCode = IOHelper::ReadAllText(vert);
        auto fragCode = IOHelper::ReadAllText(frag);
        auto cfgCode = IOHelper::ReadAllText(cfg);

        std::vector<char> stream;

        _StreamWriteSize(stream, cfgCode.size());
        stream.insert(stream.end(), cfgCode.begin(), cfgCode.end());

        glslang::InitializeProcess();

        auto vertSpv = _CompileShaderToSPIRV(vertCode.c_str(), EShLanguage::EShLangVertex, includePaths, compileInfo);
        auto fragSpv = _CompileShaderToSPIRV(fragCode.c_str(), EShLanguage::EShLangFragment, includePaths, compileInfo);

        _StreamWriteSize(stream, vertSpv.size());
        stream.insert(stream.end(), vertSpv.begin(), vertSpv.end());

        _StreamWriteSize(stream, fragSpv.size());
        stream.insert(stream.end(), fragSpv.begin(), fragSpv.end());

        glslang::FinalizeProcess();

        return stream;
    }

    void CompileToFile(const std::vector<string>& includePaths, std::string shaderFilename, std::string outputFilename, const CompileInfo& compileInfo)
    {
        auto data = CompileShader(includePaths, shaderFilename, compileInfo);
        IOHelper::WriteFile(outputFilename, data);
    }

    bool CheckFileFamily(std::filesystem::path path)
    {
        auto vs = path; vs.replace_extension(".vs.hlsl");
        auto ps = path; ps.replace_extension(".ps.hlsl");
        return std::filesystem::exists(path) && std::filesystem::exists(vs) && std::filesystem::exists(ps);
    }

    void CompileDir(const std::string& inputDir, const std::vector<string>& otherIncludePaths, const std::string& outputDir, const CompileInfo& compileInfo)
    {
        std::vector<std::filesystem::path> shcfgs;

        for (auto& path : std::filesystem::recursive_directory_iterator(inputDir))
        {
            if (!path.is_directory())
            {
                auto _path = path.path();
                if (_path.extension() == ".shcfg")
                {
                    shcfgs.push_back(_path);
                }
            }
        }

        const std::filesystem::path inputDirPath(inputDir);
        const std::filesystem::path outputDirPath(outputDir);

        for (auto& shcfgFilename : shcfgs)
        {
            if (!CheckFileFamily(shcfgFilename))
            {
                throw std::runtime_error("shader不全");
            }
            auto shaderName = shcfgFilename.stem();

            std::vector<string> includer = otherIncludePaths;

            auto dir = shcfgFilename; dir.remove_filename();
            includer.push_back(dir.string());

            auto shader = shcfgFilename; shader.replace_extension();
            auto data = CompileShader(includer, shader.string(), compileInfo);

            auto relative = dir.lexically_relative(inputDirPath);
            auto outTarget = outputDirPath; 
            if (relative != ".")
            {
                outTarget /= relative;
            }
            outTarget /= shaderName;
            outTarget += ".shader";

            IOHelper::WriteFile(outTarget.string(), data);
        }

    }


}
int main()
{
    vkshc::CompileInfo info;
    info.Debug = true;

    vkshc::CompileDir(R"(D:\Codes\LearningVk\shader)", {}, R"(D:\Codes\LearningVk\out\build\x64-Debug\shader)", info);

    return 0;
}