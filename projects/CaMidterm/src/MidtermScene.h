#pragma once
#include"SceneParent.h"
#include "Graphics/Post/CubeCoCoEffect.h"
#include "Graphics/Post/HorizontalBlur.h"
#include "Graphics/Post/VerticalBlur.h"
#include "Graphics/Post/BrightPass.h"
#include "Graphics/Post/CombinedBloom.h"
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
	entt::entity combinedBuf;

	Shader::sptr basicShader;
	Shader::sptr morphShader;

	entt::entity goldenMonkey;

	bool ambientOn = true;
	bool specularOn = true;
	bool diffuseOn = true;
	bool bloomOn = true;
	bool textureOn = true;
	bool toonOn = false;

	int toonBands = 5;
};
