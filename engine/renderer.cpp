// Source file for Renderer class.
//
// Version: 7/7/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, July 2019

//Include header file
#include "renderer.h"

//Include debug.h
#include "debug.h"

//Include glm matrix transform
#include <glm/gtc/matrix_transform.hpp>

//Include ImGui headers
#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_glfw.h"
#include "../external/imgui/imgui_impl_opengl3.h"

//Include SceneManager.h to receive scene data
#include "scenemanager.h"

//Pointer to scale vector2 for use in window_size_callback
Vec2* scale_ptr;

void window_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	//Update renderer scale
	scale_ptr->x = (float)width;
	scale_ptr->y = (float)height;
}

void Renderer::DrawSprite(Sprite* sprite, Vec2 position, Vec2 scale) {
	//Bind vertex array
	glBindVertexArray(vao);

	//Set vertices & uv data for sprite, then sub buffer the data
	float vertices[] = {
		1.0f,  1.0f, sprite->uv.rightUp.x, sprite->uv.rightUp.y,
		1.0f, 0.0f, sprite->uv.rightDown.x, sprite->uv.rightDown.y,
		0.0f,  1.0f, sprite->uv.leftUp.x, sprite->uv.leftUp.y,
		1.0f, 0.0f, sprite->uv.rightDown.x, sprite->uv.rightDown.y,
		0.0f, 0.0f, sprite->uv.leftDown.x, sprite->uv.leftDown.y,
		0.0f,  1.0f, sprite->uv.leftUp.x, sprite->uv.leftUp.y
	};

	//bind buffer and insert vertices data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind

	//Calculate the position with the camera position included
	Vec2 calculatedPos = Vec2(position.x - SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition().x,
							  position.y - SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition().y);

	//Multiply size by texture size
	Vec2 calculatedSize = Vec2(sprite->GetTexture()->textureData->width * scale.x, sprite->GetTexture()->textureData->height * scale.y);

	//Use default shader program
	glUseProgram(defaultShader->GetShaderProgram());

	//Do transformations
	glm::mat4 model(1.0);
	model = glm::translate(model, glm::vec3(calculatedPos.x, calculatedPos.y, 0.0f));  // First translate
	model = glm::scale(model, glm::vec3(calculatedSize.x, calculatedSize.y, 1.0f)); // Last scale
	defaultShader->SetMat4("model", model);

	//Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite->GetTexture()->_glTexture);

	//Draw and unbind
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::DrawText(Font * font, std::string text, Vec2 position, float scale, glm::vec3 color) {
	//Bind text VAO
	glBindVertexArray(textVao);

	// Use text shader, and set textColor uniform
	glUseProgram(textShader->GetShaderProgram());
	textShader->SetVec3("textColor", color);

	//Set active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = font->characters[*c];

		GLfloat xpos = position.x + ch.bearing.x * scale;
		GLfloat ypos = position.y + (ch.size.y - ch.bearing.y) * scale;

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
		glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind buffer

		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		position.x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}

	//Unbind
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Renderer::Renderer(Vec2 resolution, Vec2 scale, const char * title) {
	//Initialize GLFW and Glew
	if (!glfwInit()) {
		Debug::Log("GLFW Failed to Initialize");
	}

	//Set resolution and scale vectors
	this->resolution = resolution;
	this->scale = scale;

	//Set scale ptr adress
	scale_ptr = &this->scale;

	//Create GLFWWindow instance
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow((int)scale.x, (int)scale.y, title, NULL, NULL);
	glfwMakeContextCurrent(window); // Make current context

	//Window size callback
	glfwSetWindowSizeCallback(window, window_size_callback);

	//Initialize GLEW
	if (glewInit() != GLEW_OK) {
		Debug::Log("GLEW Failed to Initialize");
	}

	//Setup viewport and projection matrix
	this->projection = glm::ortho(0.0f, resolution.x, resolution.y, 0.0f, -1.0f, 1.0f);
	glViewport(0, 0, (GLsizei)scale.x, (GLsizei)scale.y);

	//Set opengl properties
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear color should be black
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);


	//Create default shader and text shader
	defaultShader = new Shader("shaders/default.vs", "shaders/default.fs");
	textShader = new Shader("shaders/text.vs", "shaders/text.fs");

	//Use default shader program, and set uniforms
	glUseProgram(defaultShader->GetShaderProgram());
	defaultShader->SetMat4("projection", projection);

	//Text shader set uniforms
	glUseProgram(textShader->GetShaderProgram());
	textShader->SetMat4("projection", projection);

	// Generate buffers for sprite rendering quad
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);

	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, NULL, GL_DYNAMIC_DRAW);

	//Setup vertex attribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


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
}

void Renderer::RegisterSprite(Sprite * sprite) {
	this->registeredSprites.push_back(sprite);
}

void Renderer::RemoveSprite(Sprite * sprite) {
	for (unsigned i = 0; i < registeredSprites.size(); i++) {
		if (registeredSprites[i] == sprite) {
			registeredSprites.erase(registeredSprites.begin() + i);
		}
	}
}

void Renderer::RegisterText(Text * text) {
	registeredTexts.push_back(text);
}

void Renderer::RemoveText(Text * text) {
	for (unsigned i = 0; i < registeredTexts.size(); i++) {
		if (registeredTexts[i] == text) {
			registeredTexts.erase(registeredTexts.begin() + i);
		}
	}
}

void Renderer::RenderFrame() {
	if (!SceneManager::GetActiveScene()->GetActiveCamera()) {
		Debug::Log("No camera present for rendering!");
		return;
	}

	//First create a new list of entities, and sort entities based on their z-index
	std::vector<Sprite*> sortedRenderList;

	//First determine highest z-index
	int highest = 0;
	for (Sprite* s : registeredSprites) {
		if (s->GetTexture() == nullptr) continue;
		if (s->GetZIndex() > highest) {
			highest = s->GetZIndex();
		}
	}

	//Now go through all "Layers"
	for (int i = 0; i <= highest; i++) {
		for (Sprite* s : registeredSprites) {
			//Todo: add test to check if owner has any relation with scene entity
			if (!s->GetOwner()) continue; // Check if sprite has a owner
			if (!s->GetOwner()->active) continue; // Check if owner is active
			if (!s->GetTexture()) continue; // Check if sprite has a texture
			if (s->GetZIndex() == i) {
				sortedRenderList.push_back(s);
			}
		}
	}

	//Now we got everything sorted so we can render the frame
	for (Sprite* s : sortedRenderList) {
		//Ui element draw, check if parent can be cast to a UIElement*
		if (dynamic_cast<UIElement*>(s->GetOwner())) {
			this->DrawSprite(s, s->GetOwner()->GetPosition() + SceneManager::GetActiveScene()->GetActiveCamera()->GetPosition(), s->GetScale() / (float)s->slices);
			continue; // Continue 
		}

		//Normal sprite drawing
		this->DrawSprite(s, s->GetOwner()->GetPosition(), s->GetScale() / (float)s->slices);
	}

	//Render text
	for (Text* t : registeredTexts) {
		this->DrawText(t->GetFont(), t->GetText(), t->GetPosition(), t->GetSize(), t->GetColor());
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

GLFWwindow * Renderer::GetWindow() {
	return window;
}

Vec2 Renderer::GetResolution() {
	return resolution;
}

Vec2 Renderer::GetScale() {
	return scale;
}

Renderer::~Renderer() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(this->window);
	glfwTerminate(); // Terminate GLFW
	Debug::Log("GLFW Terminated"); // Log
}