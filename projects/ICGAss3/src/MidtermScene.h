#pragma once
#include"SceneParent.h"
#include "Graphics/Post/CubeCoCoEffect.h"
#include "Graphics/Post/HorizontalBlur.h"
#include "Graphics/Post/VerticalBlur.h"
#include "Graphics/Post/BrightPass.h"
#include "Graphics/Post/CombinedBloom.h"
#include "Graphics/Post/GBuffer.h"
#include "Graphics/Post/DeferredLighting.h"
#include "Graphics/LUT.h"
#include <cstdlib>
#include <ctime>
class MidtermScene :
	public syre::SceneParent
{
public:
	MidtermScene(GLFWwindow* inWind);
	void Start();
	int Update();
	void ImGUIUpdate();
	Camera::sptr& GetCam();
private:
	Camera::sptr camera;

	entt::entity sceneBuff;
	entt::entity gBuff;
	entt::entity combinedBuf;
	entt::entity deferredBuff;

	Shader::sptr activeShader;
	Shader::sptr basicShader;
	Shader::sptr toonShader;
	Shader::sptr morphShader;
	Shader::sptr gBuffShader;

	entt::entity goldenMonkey;

	bool ambientOn = true;
	bool specularOn = true;
	bool diffuseOn = true;
	bool bloomOn = false;
	bool textureOn = true;
	bool toonOn = false;
	bool deferredOn = false;
	bool dispGbuffer = false;

	int gBuffTarg = 0;
	int toonBands = 5;
};
