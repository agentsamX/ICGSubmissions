#pragma once
#include "GLM/common.hpp"
struct LightVolume
{
	glm::vec4 _Position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	glm::vec4 _Colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);


	float _Radius = 1.0f;
	float _Power = 0.5f;
	int _Model = 0;
};