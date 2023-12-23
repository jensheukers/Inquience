// Source file for Renderer class.
//
// Copyright (C) Jens Heukers - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential
// Written by Jens Heukers

//Include header file
#include "renderer.h"

//Include debug.h
#include "debug.h"

//Include glm matrix transform
#include <glm/gtc/matrix_transform.hpp>

//Include ImGui headers
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//Include SceneManager.h to receive scene data
#include "scenemanager.h"

void Renderer::RenderEntity(Entity* entity, Camera* camera) {
	//Calculate the position with the camera position included
	Vec2 calculatedPos = Vec2(entity->GetGlobalPosition().x - camera->position.x, entity->GetGlobalPosition().y - camera->position.y);
	//calculatedPos = calculatedPos + Vec2((entity->GetGlobalScale().x / 2) * -camera->GetZoomScale(), (entity->GetGlobalPosition().y / 2) * -camera->GetZoomScale());

	//Try to find text component if we find a text component render the text
	if (Text* text = entity->GetComponent<Text>()) {
		RenderText(text->GetFont(), text->GetText(), calculatedPos, text->GetSize(), text->GetColor());
	}
	else {
		if (dynamic_cast<UIComponent*>(entity)) {
			calculatedPos = entity->GetGlobalPosition();
		}

		if (entity->HasComponent<Sprite>() && entity->GetComponent<Sprite>()->GetTexture()) {
			//Bind vertex array
			glBindVertexArray(vao);

			Sprite* sprite = entity->GetComponent<Sprite>();
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

			//Use default shader program
			glUseProgram(defaultShader->GetShaderProgram());

			//Do transformations
			glm::mat4 model(1.0);
			model = glm::translate(model, glm::vec3(calculatedPos.x, calculatedPos.y, 0.0f));  // First translate
			model = glm::scale(model, glm::vec3(entity->GetGlobalScale().x, entity->GetGlobalScale().y, 1.0f)); // Last scale
			defaultShader->SetMat4("model", model);

			//Bind texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, sprite->GetTexture()->_glTexture);

			//Blend Color
			defaultShader->SetVec4("blendColor", glm::vec4(sprite->blendColor._r / 255.f, sprite->blendColor._g / 255.f, sprite->blendColor._b / 255.f, sprite->blendColor._a / 255.f));

			//Draw and unbind
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	//Render children
	for (unsigned i = 0; i < entity->GetChildren().size(); i++){
		RenderEntity(entity->GetChild(i), camera);
	}
}

void Renderer::RenderText(Font* font, std::string text, Vec2 position, float size, glm::vec3 color) {
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

		GLfloat xpos = position.x + ch.bearing.x * size;
		GLfloat ypos = position.y + (ch.size.y - ch.bearing.y) * size;

		GLfloat w = ch.size.x * size;
		GLfloat h = ch.size.y * size;

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
		position.x += (ch.advance >> 6) * size; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}

	//Unbind
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Renderer::Renderer(Vec2 resolution, const char* title, bool fullscreen) {
	//Initialize GLFW and Glew
	if (!glfwInit()) {
		Debug::Log("GLFW Failed to Initialize");
	}

	//Set resolution and scale vectors
	this->resolution = resolution;

	//Create GLFWWindow instance
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	GLFWmonitor* monitor = NULL;
	if (fullscreen) monitor = glfwGetPrimaryMonitor();

	window = glfwCreateWindow((int)resolution.x, (int)resolution.y, title, monitor, NULL);
	glfwMakeContextCurrent(window); // Make current context

	//Initialize GLEW
	if (glewInit() != GLEW_OK) {
		Debug::Log("GLEW Failed to Initialize");
	}

	//Setup viewport and projection matrix
	this->projection = glm::ortho(0.0f, resolution.x, resolution.y, 0.0f, -1.0f, 1.0f);
	glViewport(0, 0, (GLsizei)resolution.x, (GLsizei)resolution.y);

	//Set opengl properties
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear color should be black
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);


	//Create default shader and text shader
	defaultShader = new Shader("shaders/default.vs", "shaders/default.fs");
	textShader = new Shader("shaders/text.vs", "shaders/text.fs");
	lineShader = new Shader("shaders/line.vs", "shaders/line.fs");
	screenShader = new Shader("shaders/screen.vs", "shaders/screen.fs");

	//Use default shader program, and set uniforms
	glUseProgram(defaultShader->GetShaderProgram());
	defaultShader->SetMat4("projection", projection);

	//Text shader set uniforms
	glUseProgram(textShader->GetShaderProgram());
	textShader->SetMat4("projection", projection);

	//Text shader set uniforms
	glUseProgram(lineShader->GetShaderProgram());
	lineShader->SetMat4("projection", projection);

	glUseProgram(screenShader->GetShaderProgram());
	screenShader->SetInt("screenTexture", 0);


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

	// screen quad VAO
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &screenVao);
	glGenBuffers(1, &screenVbo);
	glBindVertexArray(screenVao);
	glBindBuffer(GL_ARRAY_BUFFER, screenVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//Setup framebuffers
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		Debug::Log("Succesfully created framebuffer");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Create framebuffer texture
	glGenTextures(1, &fboTexture);
	glBindTexture(GL_TEXTURE_2D, fboTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resolution.x, resolution.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//attach the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

	//Create render buffer
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, resolution.x, resolution.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Debug::Log("Framebuffer is not complete!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Setup Dear ImGui context
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGuiStyle& style = ImGui::GetStyle();

	glm::vec3 color_for_text = glm::vec3(236.f / 255.f, 240.f / 255.f, 241.f / 255.f);
	glm::vec3 color_for_head = glm::vec3(41.f / 255.f, 128.f / 255.f, 185.f / 255.f);
	glm::vec3 color_for_area = glm::vec3(57.f / 255.f, 79.f / 255.f, 105.f / 255.f);
	glm::vec3 color_for_body = glm::vec3(44.f / 255.f, 62.f / 255.f, 80.f / 255.f);
	glm::vec3 color_for_pops = glm::vec3(33.f / 255.f, 46.f / 255.f, 60.f / 255.f);

	style.Colors[ImGuiCol_Text] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.58f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.95f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.58f);
	style.Colors[ImGuiCol_Border] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.47f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.21f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.80f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.50f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.50f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.32f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.15f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.43f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(color_for_pops.x, color_for_pops.y, color_for_pops.z, 0.92f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.73f);
	style.WindowRounding = 0;
	style.FrameRounding = 0;

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	//Start a new IMGUI Frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	Debug::Log("Renderer Initialized");
}

void Renderer::RenderScene(Scene* scene, Camera* camera) {
	//TODO / FIX: MAKE FRAMEBUFFER TEXTURE NOT RENDER BLACK
	// first pass
	//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//glClearColor(1, 1, 1, 1);
	//glClear(GL_COLOR_BUFFER_BIT);

	//Render all scene children as entity
	for (unsigned i = 0; i < scene->GetChildren().size(); i++) {
		RenderEntity(scene->GetChild(i), camera);
	}

	// second pass
	//glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	//glClearColor(1, 1, 1, 1);
	//glClear(GL_COLOR_BUFFER_BIT);

	//glUseProgram(screenShader->GetShaderProgram());
	//glBindVertexArray(screenVao);

	//glBindTexture(GL_TEXTURE_2D, fboTexture);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::RenderImGui() {
	//ImGui render
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::DrawLine(Vec2 a, Vec2 b, glm::vec3 color, Camera* camera) {
	//Bind vertex array
	glBindVertexArray(vao);

	//Set vertices & uv data for sprite, then sub buffer the data
	float vertices[] = {
		a.x - camera->position.x,  a.y - camera->position.y, 0.0f, 0.0f,
		b.x - camera->position.x, b.y - camera->position.y, 0.0f, 0.0f
	};

	//bind buffer and insert vertices data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind

	//Use default shader program
	glUseProgram(lineShader->GetShaderProgram());
	lineShader->SetVec3("color", color);

	//Draw and unbind
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
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
	return resolution;
}

Renderer::~Renderer() {
	//delete framebuffer
	glDeleteFramebuffers(1, &fbo);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(this->window);
	glfwTerminate(); // Terminate GLFW
	Debug::Log("GLFW Terminated"); // Log
}