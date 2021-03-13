#include "GBuffer.h"

void GBuffer::Init(unsigned width, unsigned height)
{
    int index = int(_buffers.size());
    _buffers.push_back(new Framebuffer());
    _buffers[index]->AddColorTarget(GL_RGBA8);
    _buffers[index]->AddColorTarget(GL_RGB32F);
    _buffers[index]->AddColorTarget(GL_RGB8);
    _buffers[index]->AddDepthTarget();
    _buffers[index]->Init(width, height);
    
    _shaders.push_back(Shader::Create());
    _shaders[_shaders.size() - 1]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
    _shaders[_shaders.size() - 1]->LoadShaderPartFromFile("shaders/passthrough_frag.glsl", GL_FRAGMENT_SHADER);
    _shaders[_shaders.size() - 1]->Link();
}

void GBuffer::DrawToScreen(int colBuff)
{
    BindShader(0);

    BindColorAsTexture(0, colBuff, 0);

    _buffers[0]->DrawFullscreenQuad();

    UnbindTexture(0);

    UnbindShader();
}
