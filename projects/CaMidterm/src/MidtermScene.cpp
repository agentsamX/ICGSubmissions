#include "MidtermScene.h"

MidtermScene::MidtermScene(GLFWwindow* inWind)
{
	window = inWind;
}

void MidtermScene::Start()
{
	//framebuffer and effect setup
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	sceneBuff = m_Registry.create();
	combinedBuf = m_Registry.create();

	m_Registry.emplace<PostEffect>(sceneBuff);
	m_Registry.emplace<CombinedBloom>(combinedBuf);

	m_Registry.get<PostEffect>(sceneBuff).Init(width, height);
	m_Registry.get<CombinedBloom>(combinedBuf).Init(width, height);

	//camera setup
	camera = Camera::Create();

	camera->SetPosition(glm::vec3(-6, 0, 0)); // Set initial position
	camera->SetUp(glm::vec3(0, 0, 1)); // Use a z-up coordinate system
	camera->LookAt(glm::vec3(0.0f)); // Look at center of the screen
	camera->SetFovDegrees(100.0f); // Set an initial FOV

	//shader setup
	basicShader = Shader::Create();
	basicShader->LoadShaderPartFromFile("vertex_shader.glsl", GL_VERTEX_SHADER);
	basicShader->LoadShaderPartFromFile("frag_shader.glsl", GL_FRAGMENT_SHADER);
	basicShader->Link();

	glm::vec3 lightPos = glm::vec3(-3.0f, 0.0f, -2.0f);
	glm::vec3 lightCol = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 ambientCol = glm::vec3(1.0f);

	basicShader->SetUniform("u_LightPos", lightPos);
	basicShader->SetUniform("u_LightCol", lightCol);
	basicShader->SetUniform("u_AmbientCol", ambientCol);

	//object setup
	goldenMonkey = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(goldenMonkey, "monkey_quads.obj");
	m_Registry.emplace<syre::Texture>(goldenMonkey, "goldenBright.png");
	m_Registry.emplace<syre::Transform>(goldenMonkey, glm::vec3(0.0f));

	entt::entity pedestal = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(pedestal, "pedestal.obj");
	m_Registry.emplace<syre::Texture>(pedestal, "stone-1024.png");
	m_Registry.emplace<syre::Transform>(pedestal, glm::vec3(0.0f,0.0f,-3.5f),glm::vec3(90.0f,0.f,0.0f),glm::vec3(0.4f));

	entt::entity circle = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(circle, "circlePlane.obj");
	m_Registry.emplace<syre::Texture>(circle, "dirtDark.png");
	m_Registry.emplace<syre::Transform>(circle, glm::vec3(0.0f, 0.0f, -4.0f), glm::vec3(90.0f, 0.f, 0.0f), glm::vec3(10.0f));

	entt::entity neonLight1 = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(neonLight1, "neonLightReceptacle.obj");
	m_Registry.emplace<syre::Texture>(neonLight1, "superBrightGreen.png");
	m_Registry.emplace<syre::Transform>(neonLight1, glm::vec3(0.0f, -2.0f, -3.9f), glm::vec3(90.0f, 0.f, 0.0f), glm::vec3(0.1f));

	entt::entity neonLight2 = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(neonLight2, "neonLightReceptacle.obj");
	m_Registry.emplace<syre::Texture>(neonLight2, "superBrightGreen.png");
	m_Registry.emplace<syre::Transform>(neonLight2, glm::vec3(0.0f, 2.0f, -3.9f), glm::vec3(90.0f, 0.f, 0.0f), glm::vec3(0.1f));

	entt::entity neonLight3 = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(neonLight3, "neonLightReceptacle.obj");
	m_Registry.emplace<syre::Texture>(neonLight3, "superBrightGreen.png");
	m_Registry.emplace<syre::Transform>(neonLight3, glm::vec3(2.0f, 0.0f, -3.9f), glm::vec3(90.0f, 0.f, 90.0f), glm::vec3(0.1f));

	entt::entity neonLight4 = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(neonLight4, "neonLightReceptacle.obj");
	m_Registry.emplace<syre::Texture>(neonLight4, "superBrightGreen.png");
	m_Registry.emplace<syre::Transform>(neonLight4, glm::vec3(-2.0f, 0.0f, -3.9f), glm::vec3(90.0f, 0.f, 90.0f), glm::vec3(0.1f));

	lastFrame = glfwGetTime();

}

int MidtermScene::Update()
{
	PostEffect* framebuffer = &m_Registry.get<PostEffect>(sceneBuff);
	CombinedBloom* bloom = &m_Registry.get<CombinedBloom>(combinedBuf);

	framebuffer->Clear();
	bloom->Clear();

	thisFrame = glfwGetTime();
	float deltaTime = thisFrame - lastFrame;

	framebuffer->BindBuffer(0);

	basicShader->Bind();
	basicShader->SetUniform("u_CamPos", camera->GetPosition());
	basicShader->SetUniform("u_SpecularStrength", specularOn ? 1.0f : 0.0f);
	basicShader->SetUniform("u_AmbientStrength", ambientOn ? 0.6f : 0.0f);
	basicShader->SetUniform("u_DiffuseStrength", diffuseOn ? 1.0f : 0.0f);
	basicShader->SetUniform("u_TextureOn", textureOn ? 1 : 0);
	
	glm::vec3 monkRot = m_Registry.get<syre::Transform>(goldenMonkey).GetRotation();

	m_Registry.get<syre::Transform>(goldenMonkey).SetRotation(glm::vec3(monkRot.x, monkRot.y, monkRot.z + deltaTime*100));


	auto renderView = m_Registry.view<syre::Mesh, syre::Transform, syre::Texture>();
	for (auto entity : renderView)
	{
		glm::mat4 transform = renderView.get<syre::Transform>(entity).GetModelMat();
		basicShader->SetUniformMatrix("u_ModelViewProjection", camera->GetViewProjection() * transform);
		basicShader->SetUniformMatrix("u_Model", transform);
		basicShader->SetUniformMatrix("u_ModelRotation", glm::mat3(transform));
		renderView.get<syre::Texture>(entity).Bind();
		renderView.get<syre::Mesh>(entity).Render();
	}

	framebuffer->UnBindBuffer();
	if (bloomOn)
	{

		bloom->ApplyEffect(framebuffer);

		bloom->DrawToScreen();
	}
	else
	{
		framebuffer->DrawToScreen();
	}

	camera->SetForward(glm::normalize(m_Registry.get<syre::Transform>(goldenMonkey).GetPosition() - camera->GetPosition()));

	lastFrame = thisFrame;

	return 0;
}

void MidtermScene::ImGUIUpdate()
{
	// Implementation new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	// ImGui context new frame
	ImGui::NewFrame();
	CombinedBloom* bloom = &m_Registry.get<CombinedBloom>(combinedBuf);
	float brightHold = bloom->GetThreshold();
	int passes = bloom->GetPasses();


	if (ImGui::Begin("Debug")) {
		// Render our GUI stuff
		
		glm::vec3 camPos = camera->GetPosition();

		ImGui::SliderFloat3("Camera Position", &camPos.x, -20.f, 20.f);
		ImGui::SliderFloat("Bright threshold", &brightHold, 0.0, 1.0);
		ImGui::SliderInt("Blur Passes", &passes, 0, 100);
		ImGui::Checkbox("Ambient Lighting", &ambientOn);
		ImGui::Checkbox("Diffuse Lighting", &diffuseOn);
		ImGui::Checkbox("Specular Lighting", &specularOn);
		ImGui::Checkbox("Bloom Effect", &bloomOn);

		bloom->SetThreshold(brightHold);
		bloom->SetPasses(passes);
		camera->SetPosition(camPos);


		if (ImGui::Button("1"))
		{
			ambientOn = false;
			diffuseOn = false;
			specularOn = false;
			bloomOn = false;
		}
		if (ImGui::Button("2"))
		{
			ambientOn = true;
			diffuseOn = false;
			specularOn = false;
			bloomOn = false;
		}
		if (ImGui::Button("3"))
		{
			ambientOn = false;
			diffuseOn = false;
			specularOn = true;
			bloomOn = false;
		}
		if (ImGui::Button("4"))
		{
			ambientOn = true;
			diffuseOn = true;
			specularOn = true;
			bloomOn = false;
		}
		if (ImGui::Button("5"))
		{
			ambientOn = true;
			diffuseOn = true;
			specularOn = true;
			bloomOn = true;
		}
		if (ImGui::Button("6"))
		{
			textureOn = !textureOn;
		}
		
	}
	ImGui::End();

	// Make sure ImGui knows how big our window is
	ImGuiIO& io = ImGui::GetIO();
	int width{ 0 }, height{ 0 };
	glfwGetWindowSize(window, &width, &height);
	io.DisplaySize = ImVec2((float)width, (float)height);

	// Render all of our ImGui elements
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		// Update the windows that ImGui is using
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		// Restore our gl context
		glfwMakeContextCurrent(window);
	}
}

Camera::sptr& MidtermScene::GetCam()
{
	// TODO: insert return statement here
	return camera;
}
