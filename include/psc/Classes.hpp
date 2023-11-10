#pragma once

namespace psc
{
    enum class FilePartialType : uint32_t
    {
        Sh, Vert, Pixel, Compute, Geometry, TessControl, TessEval,
    };
    inline const char* to_string(FilePartialType type)
    {
        switch (type)
        {
        case psc::FilePartialType::Sh: return "sh";
        case psc::FilePartialType::Vert: return "vs";
        case psc::FilePartialType::Pixel: return "ps";
        case psc::FilePartialType::Compute: return "cs";
        case psc::FilePartialType::Geometry: return "gs";
        case psc::FilePartialType::TessControl: return "tcs";
        case psc::FilePartialType::TessEval: return "tes";
        }
        return {};
    }
    enum SourceFileType : uint32_t
    {
        SFT_none,
        SFT_glsl = 1,
        SFT_hlsl = 1 << 1,
        SFT_yaml = 1 << 2,
        SFT_json = 1 << 3,
        SFT_xml = 1 << 4,
        SFT_toml = 1 << 5,
        SFT_ShaderSrc = SFT_glsl | SFT_hlsl,
        SFT_Config = SFT_yaml | SFT_json | SFT_xml | SFT_toml
    };
    using PlatformType = uint32_t;
    enum PlatformType_ : uint32_t
    {
        PlatformType_None,
        PlatformType_Generic = 1,
        PlatformType_Vulkan = 1 << 1,
        PlatformType_Direct3D = 1 << 2,
        PlatformType_Metal = 1 << 3,
        PlatformType_OpenGL = 1 << 4,
        PlatformType_ALL = -1,
    };
    inline const char* to_string(PlatformType_ type)
    {
        switch (type)
        {
        case psc::PlatformType_None: return "None";
        case psc::PlatformType_Vulkan: return "Vulkan";
        case psc::PlatformType_Direct3D: return "Direct3D";
        case psc::PlatformType_Metal: return "Metal";
        case psc::PlatformType_OpenGL: return "OpenGL";
        }
        return nullptr;
    }


    constexpr const char* ShaderFileMagic = "plsrsh";
}