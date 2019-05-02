// Source file for renderer class.
//
// Version: 1/5/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, May 2019
#include <glm/gtc/matrix_transform.hpp>
#include "scenemanager.h"
#include "renderer.h"
#include "entity.h"
#include "debug.h"

//Include imgui
#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_glfw.h"
#include "../external/imgui/imgui_impl_opengl3.h"

void Renderer::DrawSprite(Texture* texture, Vec2 position, Vec2 size, float rotation, SpriteUV uvData) {
	//Make sure texture is not nullptr
	if (texture == nullptr) return; // Return if true

	//Sub-Buffer data
	float vertices[] = {
		1.0f,  1.0f, 0.0f, uvData.rightUp.x, uvData.rightUp.y,
		1.0f, 0.0f, 0.0f, uvData.rightDown.x, uvData.rightDown.y,
		0.0f,  1.0f, 0.0f, uvData.leftUp.x, uvData.leftUp.y,
		1.0f, 0.0f, 0.0f, uvData.rightDown.x, uvData.rightDown.y,
		0.0f, 0.0f, 0.0f, uvData.leftDown.x, uvData.leftDown.y,
		0.0f,  1.0f, 0.0f, uvData.leftUp.x, uvData.leftUp.y
	};

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

	//Calculate the position with camera position
	Vec2 calculatedPos = Vec2(position.x - SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition().x, 
							  position.y - SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition().y);

	//Calculate size, make it equal texture size
	Vec2 calculatedSize = Vec2(texture->textureData->width * size.x, texture->textureData->height * size.y);

	//Use default shader program
	glUseProgram(defaultShader->GetShaderProgram());

	glm::mat4 model(1.0);
	model = glm::translate(model, glm::vec3(calculatedPos.ToGLM(), 0.0f));  // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

	model = glm::translate(model, glm::vec3(0.5f * calculatedSize.x, 0.5f * calculatedSize.y, 0.0f)); // Move origin of rotation to center of quad
	model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f)); // Then rotate
	model = glm::translate(model, glm::vec3(-0.5f * calculatedSize.x, -0.5f * calculatedSize.y, 0.0f)); // Move origin back

	model = glm::scale(model, glm::vec3(calculatedSize.ToGLM(), 1.0f)); // Last scale
	defaultShader->SetMat4("model", model);

	//Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->_glTexture);

	//Draw
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

int Renderer::Initialize(int width, int height, const char* title) {
	//Initialize GLFW
	if (!glfwInit()) {
		Debug::Log("GLFW Failed to Initialize");
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create glfw window
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	glfwMakeContextCurrent(window); // Make current context
	
	screenResolution = Vec2((float)width, (float)height);

	//Initialize GLEW
	if (glewInit() != GLEW_OK) {
		Debug::Log("GLEW Failed to Initialize");
		return 1;
	}

	//Set opengl properties
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear color should be black
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);

	//Create default shader
	defaultShader = new Shader("shaders/default.vs","shaders/default.fs");

	//Use default shader program, to set some uniforms
	glUseProgram(defaultShader->GetShaderProgram());

	glm::mat4 projection = glm::ortho(0.0f, screenResolution.x, screenResolution.y, 0.0f, -1.0f, 1.0f);
	defaultShader->SetMat4("projection", projection);

	//Setup quad VBO and VAO
	float vertices[] = {
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f,  1.0f, 0.0f, 0.0f, 1.0f
	};

	//Create quad VBO and VAO
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);

	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	
	//Setup vertex attribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Setup Dear ImGui context
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	//Start a new IMGUI Frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	Debug::Log("Renderer Initialized");
	return 0;
}

void Renderer::RegisterEntity(Entity* entity) {
	this->renderList.push_back(entity);
}

void Renderer::RemoveEntity(Entity* entity) {
	for (int i = 0; i < (int)renderList.size(); i++) {
		if (renderList[i] == entity) {
			renderList.erase(renderList.begin() + i);
		}
	}
}

void Renderer::RenderFrame() {
	if (!SceneManager::GetActiveScene()->GetActiveCamera()) {
		Debug::Log("No camera present for rendering!");
		return;
	}

	//First create a new list of entities, and sort entities based on their z-index
	std::vector<Entity*> sortedRenderList;

	//First determine highest z-index
	int highest = 0;
	for (Entity* e : renderList) {
		if (!e->HasComponent<Sprite>()) continue;
		if (e->GetComponent<Sprite>()->GetZIndex() > highest) {
			highest = e->GetComponent<Sprite>()->GetZIndex();
		}
	}

	//Now go through all "Layers"
	for (int i = 0; i <= highest; i++) {
		for (Entity* e : renderList) {
			if (!e->HasComponent<Sprite>()) continue;
			if (e->GetComponent<Sprite>()->GetZIndex() == i) {
				sortedRenderList.push_back(e);
			}
		}
	}

	//Now we got everything sorted so we can render the frame
	for (Entity* e : sortedRenderList) {
		this->DrawSprite(e->GetComponent<Sprite>()->GetTexture(), e->GetPosition(), e->GetComponent<Sprite>()->GetScale() / (float)e->GetComponent<Sprite>()->GetSplits(), e->localRotation, e->GetComponent<Sprite>()->uvCoordinates);
	}

	//ImGui render
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::PollEvents() {
	glfwPollEvents();
}

void Renderer::SwapBuffers() {
	glfwSwapBuffers(window);
}

void Renderer::Clear() {
	glClear(GL_COLOR_BUFFER_BIT);

	//We start a new imgui frame here so we can draw in between frames
	ImGui::EndFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

GLFWwindow* Renderer::GetWindow() {
	return window;
}

Renderer::~Renderer() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(this->window);
	glfwTerminate(); // Terminate GLFW
	Debug::Log("GLFW Terminated"); // Log
}