#pragma once
#include "Graphics/Post/PostEffect.h"
class GBuffer : public PostEffect
{
public:
	void Init(unsigned width, unsigned height) override;
	void DrawToScreen(int colBuff);



};