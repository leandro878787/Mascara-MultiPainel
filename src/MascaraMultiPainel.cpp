#include "MascaraMultiPainel.h"
#include <string>
using namespace ffglex;

enum ParamType : FFUInt32
{
    PT_MODE = 0,
    PT_PANELS,
    PT_GAP,
    PT_SHAPE,
    PT_GLOBAL_SIZE,
    PT_PANEL_WIDTH_BASE,
    PT_PANEL_HEIGHT_BASE = PT_PANEL_WIDTH_BASE + 16,
    PT_PARAM_COUNT = PT_PANEL_HEIGHT_BASE + 16
};

static CFFGLPluginInfo PluginInfo(
    PluginFactory<MascaraMultiPainel>,
    "MMP1",
    "Máscara MultiPainel",
    2, 1,
    0, 1,
    FF_EFFECT,
    "Divide a imagem em painéis com dimensões ajustáveis individualmente.",
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
uniform float GlobalSize;
uniform float PanelWidth[16];
uniform float PanelHeight[16];
in vec2 uv;
out vec4 fragColor;

void main()
{
    int count = int(clamp(floor(Panels + 0.5), 1.0, 16.0));
    float baseCell = GlobalSize / float(count);
    float gapWidth = clamp(Gap, 0.0, 0.45) * baseCell;

    float totalWidth = gapWidth * float(max(count - 1, 0));
    float sourceWidth = 0.0;
    for (int i = 0; i < 16; ++i)
    {
        if (i < count)
        {
            float w = baseCell * clamp(PanelWidth[i], 0.25, 2.0);
            totalWidth += w;
            sourceWidth += w;
        }
    }

    // Mantem todos os paineis dentro da tela, preservando as proporcoes.
    float fitScale = min(1.0, 0.98 / max(totalWidth, 0.001));
    float x = 0.5 - (totalWidth * fitScale) * 0.5;
    float sourceOffset = 0.0;

    for (int i = 0; i < 16; ++i)
    {
        if (i < count)
        {
            float panelW = baseCell * clamp(PanelWidth[i], 0.25, 2.0) * fitScale;
            float panelH = clamp(PanelHeight[i] * GlobalSize, 0.05, 1.0);
            float panelStart = x;
            float panelEnd = x + panelW;

            if (uv.x >= panelStart && uv.x <= panelEnd)
            {
                float localX = (uv.x - panelStart) / max(panelW, 0.0001);
                float yStart = 0.5 - panelH * 0.5;
                float yEnd = 0.5 + panelH * 0.5;
                float inside = step(yStart, uv.y) * step(uv.y, yEnd);

                if (Shape >= 0.5)
                {
                    vec2 p = vec2((localX - 0.5) * 2.0, ((uv.y - 0.5) / max(panelH * 0.5, 0.001)));
                    inside *= 1.0 - step(1.0, dot(p, p));
                }

                vec2 sampleUV = uv;
                if (Mode >= 0.5)
                    sampleUV.x = localX;
                else
                    sampleUV.x = (sourceOffset + localX * (panelW / fitScale)) / max(sourceWidth, 0.001);

                vec4 color = texture(InputTexture, sampleUV);
                fragColor = color * inside;
                return;
            }

            x = panelEnd + gapWidth * fitScale;
            sourceOffset += panelW / fitScale;
        }
    }

    fragColor = vec4(0.0);
}
)";

MascaraMultiPainel::MascaraMultiPainel()
    : mode(0.0f), panels(4.0f), gap(0.06f), shape(0.0f), globalSize(1.0f)
{
    for (int i = 0; i < 16; ++i)
    {
        panelWidth[i] = 1.0f;
        panelHeight[i] = 1.0f;
    }

    SetMinInputs(1);
    SetMaxInputs(1);
    SetParamInfof(PT_MODE, "Modo", FF_TYPE_STANDARD);
    SetParamInfof(PT_PANELS, "Quantidade", FF_TYPE_STANDARD);
    SetParamInfof(PT_GAP, "Espacamento", FF_TYPE_STANDARD);
    SetParamInfof(PT_SHAPE, "Formato", FF_TYPE_STANDARD);
    SetParamInfof(PT_GLOBAL_SIZE, "Tamanho geral", FF_TYPE_STANDARD);

    for (int i = 0; i < 16; ++i)
    {
        char widthName[17];
        char heightName[17];
        std::snprintf(widthName, sizeof(widthName), "Largura P%02d", i + 1);
        std::snprintf(heightName, sizeof(heightName), "Altura P%02d", i + 1);
        SetParamInfof(PT_PANEL_WIDTH_BASE + i, widthName, FF_TYPE_STANDARD);
        SetParamInfof(PT_PANEL_HEIGHT_BASE + i, heightName, FF_TYPE_STANDARD);
    }
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
    shader.Set("GlobalSize", globalSize);

    for (int i = 0; i < 16; ++i)
    {
        const std::string widthUniform = "PanelWidth[" + std::to_string(i) + "]";
        const std::string heightUniform = "PanelHeight[" + std::to_string(i) + "]";
        shader.Set(widthUniform.c_str(), panelWidth[i]);
        shader.Set(heightUniform.c_str(), panelHeight[i]);
    }

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
    case PT_GLOBAL_SIZE: globalSize = std::clamp(value, 0.25f, 1.5f); break;
    default:
        if (index >= PT_PANEL_WIDTH_BASE && index < PT_PANEL_HEIGHT_BASE)
            panelWidth[index - PT_PANEL_WIDTH_BASE] = std::clamp(value, 0.25f, 2.0f);
        else if (index >= PT_PANEL_HEIGHT_BASE && index < PT_PARAM_COUNT)
            panelHeight[index - PT_PANEL_HEIGHT_BASE] = std::clamp(value, 0.25f, 2.0f);
        else
            return FF_FAIL;
        break;
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
    case PT_GLOBAL_SIZE: return globalSize;
    default:
        if (index >= PT_PANEL_WIDTH_BASE && index < PT_PANEL_HEIGHT_BASE)
            return panelWidth[index - PT_PANEL_WIDTH_BASE];
        if (index >= PT_PANEL_HEIGHT_BASE && index < PT_PARAM_COUNT)
            return panelHeight[index - PT_PANEL_HEIGHT_BASE];
        return 0.0f;
    }
}
