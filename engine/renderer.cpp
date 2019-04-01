// Source file for renderer class.
//
// Version: 26/3/2019
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers, March 2019
#include "renderer.h"
#include "debug.h"

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
	
	screenResolution = Vec2(width, height);

	//Initialize GLEW
	if (glewInit() != GLEW_OK) {
		Debug::Log("GLEW Failed to Initialize");
		return 1;
	}

	//Set opengl properties
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear color should be black

	//Create default shader
	defaultShader = new Shader("shaders/default.vs","shaders/default.fs");

	Debug::Log("Renderer Initialized");
	return 0;
}

void Renderer::DrawSprite(Sprite* sprite, Vec2 position) {
	Texture* texture = sprite->GetTexture();
	if (texture == nullptr) return;

	float tx = (((float)texture->width / screenResolution.x) * 2) - 1.f;
	float ty = (((float)texture->height / screenResolution.y) * 2) - 1.f;

	float quadVertices[] = {
		-1.0f,  ty,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		tx, -1.0f,  1.0f, 0.0f,

		-1.0f,  ty,  0.0f, 1.0f,
		tx, -1.0f,  1.0f, 0.0f,
		tx,  ty,  1.0f, 1.0f
	};

	//Check if vao and vbo are not set, if not we generate buffers
	if (vao == 0 && vbo == 0) {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	}
	else {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), &quadVertices);
	}

	glDisable(GL_DEPTH_TEST); // Disable depth testing

	glUseProgram(defaultShader->GetShaderProgram()); // Bind framebuffer shader program
	glBindVertexArray(vao); // Bind Vertex Array Object

								  //Normalize positions to get OpenGL coordinates
	float x = (position.x / screenResolution.x) * 2;
	float y = (position.y / screenResolution.y) * 2;

	//Set uniforms
	defaultShader->SetVec2("position", glm::vec2(x, y));
	defaultShader->SetVec2("scale", glm::vec2(1, 1));

	glBindTexture(GL_TEXTURE_2D, texture->textureID); // Bind framebuffer texture
	glDrawArrays(GL_TRIANGLES, 0, 6); // Draw quad
}

void Renderer::PollEvents() {
	glfwPollEvents();
}

void Renderer::SwapBuffers() {
	glfwSwapBuffers(window);
}

void Renderer::Clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}

GLFWwindow* Renderer::GetWindow() {
	return window;
}