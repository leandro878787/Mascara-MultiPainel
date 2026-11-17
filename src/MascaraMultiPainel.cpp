#include "MascaraMultiPainel.h"
using namespace ffglex;

enum ParamType : FFUInt32
{
    PT_MODE,
    PT_PANELS,
    PT_GAP,
    PT_SHAPE
};

static CFFGLPluginInfo PluginInfo(
    PluginFactory<MascaraMultiPainel>,
    "MMP1",
    "Máscara MultiPainel",
    2, 1,
    0, 1,
    FF_EFFECT,
    "Divide a imagem em painéis com modo contínuo ou duplicado.",
    "Desenvolvido por @leandrone_x | Instagram: @leandrone_x"
);

static const char vertexShader[] = R"(#version 410 core
uniform vec2 MaxUV;
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec2 vUV;
out vec2 uv;
void main()
{
    gl_Position = vPosition;
    uv = vUV * MaxUV;
}
)";

static const char fragmentShader[] = R"(#version 410 core
uniform sampler2D InputTexture;
uniform float Mode;
uniform float Panels;
uniform float Gap;
uniform float Shape;
in vec2 uv;
out vec4 fragColor;

void main()
{
    float count = clamp(floor(Panels + 0.5), 1.0, 16.0);
    float cellX = uv.x * count;
    float localX = fract(cellX);
    float safeGap = clamp(Gap, 0.0, 0.45);
    float halfGap = safeGap * 0.5;

    // Mode 0: panorama contínuo. Mode 1: imagem inteira repetida em cada painel.
    vec2 sampleUV = uv;
    if (Mode >= 0.5)
        sampleUV.x = localX;

    vec4 color = texture(InputTexture, sampleUV);

    float inside = step(halfGap, localX) * step(localX, 1.0 - halfGap);

    // Shape 0: painel retangular. Shape 1: painel circular/elíptico.
    if (Shape >= 0.5)
    {
        vec2 p = vec2((localX - 0.5) / max(0.5 - halfGap, 0.001), (uv.y - 0.5) / 0.5);
        inside *= 1.0 - step(1.0, dot(p, p));
    }

    color *= inside;
    fragColor = color;
}
)";

MascaraMultiPainel::MascaraMultiPainel()
    : mode(0.0f), panels(4.0f), gap(0.06f), shape(0.0f)
{
    SetMinInputs(1);
    SetMaxInputs(1);
    SetParamInfof(PT_MODE, "Modo", FF_TYPE_STANDARD);
    SetParamInfof(PT_PANELS, "Quantidade de paineis", FF_TYPE_STANDARD);
    SetParamInfof(PT_GAP, "Espaco entre paineis", FF_TYPE_STANDARD);
    SetParamInfof(PT_SHAPE, "Formato", FF_TYPE_STANDARD);
}

FFResult MascaraMultiPainel::InitGL(const FFGLViewportStruct* vp)
{
    if (!shader.Compile(vertexShader, fragmentShader))
    {
        DeInitGL();
        return FF_FAIL;
    }
    if (!quad.Initialise())
    {
        DeInitGL();
        return FF_FAIL;
    }
    return CFFGLPlugin::InitGL(vp);
}

FFResult MascaraMultiPainel::ProcessOpenGL(ProcessOpenGLStruct* pGL)
{
    if (pGL->numInputTextures < 1 || pGL->inputTextures[0] == nullptr)
        return FF_FAIL;

    ScopedShaderBinding shaderBinding(shader.GetGLID());
    ScopedSamplerActivation activateSampler(0);
    Scoped2DTextureBinding textureBinding(pGL->inputTextures[0]->Handle);

    shader.Set("InputTexture", 0);
    FFGLTexCoords maxCoords = GetMaxGLTexCoords(*pGL->inputTextures[0]);
    shader.Set("MaxUV", maxCoords.s, maxCoords.t);
    shader.Set("Mode", mode);
    shader.Set("Panels", panels);
    shader.Set("Gap", gap);
    shader.Set("Shape", shape);
    quad.Draw();
    return FF_SUCCESS;
}

FFResult MascaraMultiPainel::DeInitGL()
{
    shader.FreeGLResources();
    quad.Release();
    return FF_SUCCESS;
}

FFResult MascaraMultiPainel::SetFloatParameter(unsigned int index, float value)
{
    switch (index)
    {
    case PT_MODE: mode = value < 0.5f ? 0.0f : 1.0f; break;
    case PT_PANELS: panels = (value < 1.0f) ? 1.0f : ((value > 16.0f) ? 16.0f : value); break;
    case PT_GAP: gap = (value < 0.0f) ? 0.0f : ((value > 0.45f) ? 0.45f : value); break;
    case PT_SHAPE: shape = value < 0.5f ? 0.0f : 1.0f; break;
    default: return FF_FAIL;
    }
    return FF_SUCCESS;
}

float MascaraMultiPainel::GetFloatParameter(unsigned int index)
{
    switch (index)
    {
    case PT_MODE: return mode;
    case PT_PANELS: return panels;
    case PT_GAP: return gap;
    case PT_SHAPE: return shape;
    default: return 0.0f;
    }
}
