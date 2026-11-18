#pragma once
#include <FFGLSDK.h>

class MascaraMultiPainel : public CFFGLPlugin
{
public:
    MascaraMultiPainel();
    FFResult InitGL(const FFGLViewportStruct* vp) override;
    FFResult ProcessOpenGL(ProcessOpenGLStruct* pGL) override;
    FFResult DeInitGL() override;
    FFResult SetFloatParameter(unsigned int index, float value) override;
    float GetFloatParameter(unsigned int index) override;

private:
    ffglex::FFGLShader shader;
    ffglex::FFGLScreenQuad quad;
    float mode;
    float panels;
    float gap;
    float shape;
    float globalSize;
    float panelWidth[16];
    float panelHeight[16];
};
