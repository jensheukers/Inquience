// Source file for renderer class.
//
// Version: 4/7/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, July 2019
#include <glm/gtc/matrix_transform.hpp>
#include "scenemanager.h"
#include "renderer.h"
#include "entity.h"
#include "debug.h"

//Include core.h to get instance for callback
#include "core.h"

//Include imgui
#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_glfw.h"
#include "../external/imgui/imgui_impl_opengl3.h"

void window_size_callback(GLFWwindow* window, int width, int height)
{
	Core::GetRendererInstance()->WindowSizeCallback(window, width, height);
}

void Renderer::DrawSprite(Sprite* sprite, Vec2 position, Vec2 size) {
	glBindVertexArray(this->vao);

	//Sub-Buffer data
	float vertices[] = {
		1.0f,  1.0f, 0.0f, sprite->uv.rightUp.x, sprite->uv.rightUp.y,
		1.0f, 0.0f, 0.0f, sprite->uv.rightDown.x, sprite->uv.rightDown.y,
		0.0f,  1.0f, 0.0f, sprite->uv.leftUp.x, sprite->uv.leftUp.y,
		1.0f, 0.0f, 0.0f, sprite->uv.rightDown.x, sprite->uv.rightDown.y,
		0.0f, 0.0f, 0.0f, sprite->uv.leftDown.x, sprite->uv.leftDown.y,
		0.0f,  1.0f, 0.0f, sprite->uv.leftUp.x, sprite->uv.leftUp.y
	};

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

	//Calculate the position with camera position
	Vec2 calculatedPos = Vec2(position.x - SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition().x, 
							  position.y - SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition().y);

	//Calculate size, make it equal texture size
	Vec2 calculatedSize = Vec2(sprite->GetTexture()->textureData->width * size.x, sprite->GetTexture()->textureData->height * size.y);

	//Use default shader program
	glUseProgram(defaultShader->GetShaderProgram());

	glm::mat4 model(1.0);
	model = glm::translate(model, glm::vec3(calculatedPos.ToGLM(), 0.0f));  // First translate
	model = glm::scale(model, glm::vec3(calculatedSize.ToGLM(), 1.0f)); // Last scale
	defaultShader->SetMat4("model", model);

	//Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite->GetTexture()->_glTexture);

	//Draw
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Renderer::DrawText(Font* font, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
	// Activate corresponding render state	
	glUseProgram(textShader->GetShaderProgram());
	textShader->SetVec3("textColor", color);

	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(textVao);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = font->characters[*c];

		GLfloat xpos = x + ch.bearing.x * scale;
		GLfloat ypos = y + (ch.size.y - ch.bearing.y) * scale;

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;

		GLfloat vertices[6][4] = {
			{ xpos,     ypos - h, 0.0, 0.0 },
			{ xpos,     ypos,     0.0, 1.0 },
			{ xpos + w, ypos,     1.0, 1.0 },

			{ xpos,     ypos - h,  0.0, 0.0 },
			{ xpos + w, ypos,      1.0, 1.0 },
			{ xpos + w, ypos - h,  1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, textVbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
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
	
	glfwSetWindowSizeCallback(window, window_size_callback);

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

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	//Create default shader and text shader
	defaultShader = new Shader("shaders/default.vs","shaders/default.fs");
	textShader = new Shader("shaders/text.vs", "shaders/text.fs");

	//Use default shader program, to set some uniforms
	glUseProgram(defaultShader->GetShaderProgram());

	glm::mat4 projection = glm::ortho(0.0f, screenResolution.x, screenResolution.y, 0.0f, -1.0f, 1.0f);
	defaultShader->SetMat4("projection", projection);

	//Text shader
	glUseProgram(textShader->GetShaderProgram());
	textShader->SetMat4("projection", projection);

	// Generate buffers for text rendering quad
	glGenVertexArrays(1, &textVao);
	glGenBuffers(1, &textVbo);

	glBindVertexArray(textVao);
	glBindBuffer(GL_ARRAY_BUFFER, textVbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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
	glBindVertexArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

void Renderer::RegisterText(Text* text) {
	this->textList.push_back(text);
}

void Renderer::RemoveText(Text* text) {
	for (int i = 0; i < (int)textList.size(); i++) {
		if (textList[i] == text) {
			textList.erase(textList.begin() + i);
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
		if (!e->GetComponent<Sprite>()->GetTexture()) continue;
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
		this->DrawSprite(e->GetComponent<Sprite>(), e->GetPosition(), e->GetComponent<Sprite>()->GetScale());
	}

	//Render text
	for (Text* t : textList) {
		this->DrawText(t->GetFont(), t->GetText(), t->GetPosition().x, t->GetPosition().y, t->GetSize(), t->GetColor());
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

Vec2 Renderer::GetResolution() {
	return screenResolution;
}

void Renderer::WindowSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	screenResolution = Vec2((float)width, (float)height);
}

Renderer::~Renderer() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(this->window);
	glfwTerminate(); // Terminate GLFW
	Debug::Log("GLFW Terminated"); // Log
}