#include "HorizontalBlur.h"

void HorizontalBlur::Init(unsigned width, unsigned height)
{
    int index = int(_buffers.size());
    _buffers.push_back(new Framebuffer());
    _buffers[index]->AddColorTarget(GL_RGBA8);
    _buffers[index]->AddDepthTarget();
    _buffers[index]->Init(width, height);

    //load shader
    index = int(_shaders.size());
    _shaders.push_back(Shader::Create());
    _shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
    _shaders[index]->LoadShaderPartFromFile("shaders/passthrough_frag.glsl", GL_FRAGMENT_SHADER);
    _shaders[index]->Link();
    index = int(_shaders.size());
    _shaders.push_back(Shader::Create());
    _shaders[index]->LoadShaderPartFromFile("shaders/Post/horizontalBlur_vert.glsl", GL_VERTEX_SHADER);
    _shaders[index]->LoadShaderPartFromFile("shaders/Post/horizontalBlur_frag.glsl", GL_FRAGMENT_SHADER);
    _shaders[index]->Link();
   
}

void HorizontalBlur::ApplyEffect(PostEffect* buffer)
{
    BindShader(1);
    _shaders[1]->SetUniform("u_width", 1280.f);
    buffer->BindColorAsTexture(0, 0, 0);

    _buffers[0]->RenderToFSQ();
    
    buffer->UnbindTexture(0);

    UnbindShader();
}

void HorizontalBlur::DrawToScreen()
{
    BindShader(0);

    BindColorAsTexture(0, 0, 0);

    _buffers[0]->DrawFullscreenQuad();

    UnbindTexture(0);

    UnbindShader();
}
