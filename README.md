# ShaderCompiler

Shader编译工具，支持将hlsl编译为SPIR-V字节码，并打包为一个二进制shader文件。

被编译工具视为一个Shader可能的文件有
- Lit.sh.json (任意文本，主要用于配置文件)
- Lit.vs.hlsl (Vertex Shader)
- Lit.ps.hlsl (Pixel Shader)
- Lit.cs.hlsl (Compute Shader)
- Lit.gs.hlsl (Geometry Shader)
- Lit.tcs.hlsl (TessControl Shader)
- Lit.tes.hlsl (TessEval Shader)

编译工具不会解析.sh.json的内容，而是直接拷贝进最后的编译文件中。
最后编译文件将储存着色器字节码与json文本，每个数据前拥有用于标记类型的数字和数据长度。

一个shader文件的完整结构
- 文本文件长度 (4 Byte)
- 文本文件内容 (长度)
- 平台类型 如ogl dx等 (1 Byte)
- 该平台的数据块大小，用该数据可以快速调整流读取位置 (4 Byte)
  - 数据阶段类型，如vs，ps (1 Byte)
  - 数据长度 (4 Byte)
  - 数据内容 (数据长度)
  - 数据阶段类型，如vs，ps (1 Byte)
  - 数据长度 (4 Byte)
  - 数据内容 (数据长度)
  - ...
- 平台类型 (1 Byte)
  - ...

平台类型：
- Vulkan: 0
- Direct3D: 1
- Metal : 2
- OpenGL: 3

数据阶段类型：
- Json：0
- Vertex: 1
- Pixel: 2
- Compute: 3
- Geometry: 4
- TessControl : 5
- TessEval : 6

## Usage
```
    --debug                binary with debug info (default: true)
-h, --help                 help
-i, --include FOLDERS;...  include folder
-s, --source PATH          shader json source
-c, --compile PATH         compiling output filename
-d, --defines KEY=VALUE    predefine macros
```
编译shader时，shader所在文件夹会被默认添加到include列表当中，同时也可以通过-i来指定更多文件夹。  
例：
`ShaderCompiler.exe -i ".\Deferred" -i ".\Common" -s "Missing.sh.json" -c "Missing.shader"`