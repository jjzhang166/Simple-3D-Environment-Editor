#include "stdafx.hpp"

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Editable Elements
ImVec4 lightPosition(0.0f, 30.0f, -100.0f, 0.0f);
ImVec4 skyColor(0.53f, 0.8f, 0.92f, 1.0f);
ImVec4 lightColor(1.0f, 1.0f, 1.0f, 0.0f);
ImVec4 objectColor(0.1f, 0.6f, 0.1f, 0.0f);
float ambientStrength = 0.80f;
float specularStrength = 0.25f;
int reflectivity = 8;


// Declare Callback Functions
void processInput(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void framebufferCallback(GLFWwindow* window, int width, int height);

// main function/entry point
int main() {
	// Init and Set Window settings
	if (!glfwInit()) {
		return EXIT_FAILURE;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Square Triangle Coordinates
	float triangles[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,

		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,

		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f
	};

	GLuint indices[] = {
		// Square Indice Order
		0, 1, 2,  // Triangle 1
		0, 2, 3,  // Triangle 2
		4, 5, 6,  // etc.
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23
	};

	// Create the window and set it to the current context
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Lighting", NULL, NULL);
	if (window == NULL) {
		LOG("ERROR: Failed to create GLFW window.");
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// Configure the Window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetCursorPos(window, camera.lastX, camera.lastY);
	glfwSwapInterval(1);

	// Load the GL Functions and set the viewport of the window
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	if (!gladLoadGL()) {
		LOG("ERROR: Failed to initialize GLAD.");
		glfwDestroyWindow(window);
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// Init ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	// Configure the Texture Drawing Formats
	glEnable(GL_DEPTH_TEST);

	// Create Object & Light Shaders
	Shader objectShader("shaders/vertexShader.vs", "shaders/fragObjectShader.fs");
	Shader lightShader("shaders/vertexShader.vs", "shaders/fragLightShader.fs");

	// Create VBOs & VAOs
	VAO objectVAO, lightVAO;
	VBO VBO;
	EBO EBO;

	// Read in the triangle
	VBO.init(triangles, sizeof(triangles));
	EBO.init(indices, sizeof(indices));

	// Cube Object VAO
	objectVAO.bind();
	VBO.bind();
	EBO.bind();
	objectVAO.linkVBO(VBO, 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	objectVAO.linkVBO(VBO, 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	objectVAO.unbind();
	VBO.unbind();
	EBO.unbind();

	// Light Source VAO
	lightVAO.bind();
	VBO.bind();
	EBO.bind();
	lightVAO.linkVBO(VBO, 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	lightVAO.linkVBO(VBO, 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	lightVAO.unbind();
	VBO.unbind();
	EBO.unbind();

	// Create Ground (The object we will see)
	glm::mat4 model(1.0);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(110.0f, 1.0f, 110.0f));
	objectShader.setMat4("model", model);
	lightShader.setMat4("model", model);

	// Normal Matrix for Lighting Calculations
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(model));
	objectShader.setMat3("normalMatrix", normalMatrix);
	lightShader.setMat3("normalMatrix", normalMatrix);

	// Set the sky color buffer for wiping/resetting the window
	glClearColor(skyColor.x, skyColor.y, skyColor.z, skyColor.w);

	while (!glfwWindowShouldClose(window)) {
		// Update the Time
		double currentFrame = glfwGetTime();
		deltaTime = (float) currentFrame - lastFrame;
		lastFrame = (float) currentFrame;

		// Process Input
		glfwPollEvents();
		processInput(window);

		// Clear the Buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set the Lighting Values
		objectShader.setVec3("objectColor", glm::vec3(objectColor.x, objectColor.y, objectColor.z));
		objectShader.setVec3("lightColor", glm::vec3(lightColor.x, lightColor.y, lightColor.z));
		objectShader.setVec3("lightPos", glm::vec3(lightPosition.x, lightPosition.y, lightPosition.z));
		objectShader.setVec3("viewPos", camera.position);
		objectShader.setFloat("ambientStrength", ambientStrength);
		objectShader.setFloat("specularStrength", specularStrength);
		objectShader.setInt("reflectivity", reflectivity);

		// Calculate Camera Position/View
		glm::mat4 view = camera.getViewMatrix();
		objectShader.setMat4("view", view);
		lightShader.setMat4("view", view);

		// Projection Matrix (Depth Perception)
		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float) WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 150.0f);
		objectShader.setMat4("projection", projection);
		lightShader.setMat4("projection", projection);

		// Draw the Object
		objectShader.activate();
		objectVAO.bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		
		// Draw the Light Source
		glm::mat4 light(1.0);
		light = glm::translate(light, glm::vec3(lightPosition.x, lightPosition.y, lightPosition.z));
		light = glm::scale(light, glm::vec3(4.0f, 4.0f, 4.0f));
		light = glm::rotate(light, 45.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		lightShader.setMat4("model", light);
		lightShader.setVec3("lightColor", glm::vec3(lightColor.x, lightColor.y, lightColor.z));
		lightShader.activate();
		lightVAO.bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			ImGui::Begin("Scene Editor");
			ImGui::ColorEdit3("Sky Color", (float*) &skyColor);
			ImGui::ColorEdit3("Ground Color", (float*) &objectColor);
			ImGui::ColorEdit3("Light Color", (float*) &lightColor);
			ImGui::InputFloat3("Light Position", (float*) &lightPosition);
			ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0.0f, 1.0f, "%.2f", 1);
			ImGui::SliderFloat("Specular Strength", &specularStrength, 0.0f, 1.0f, "%.2f", 1);
			ImGui::InputInt("Reflectivity", &reflectivity, 2);
			ImGui::SetWindowSize(ImVec2(400.0f, 200.0f));
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update the Elements
		glClearColor(skyColor.x, skyColor.y, skyColor.z, skyColor.w);
		glfwSwapBuffers(window);
	}

	// Clean Up and Close
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	objectVAO.destroy();
	lightVAO.destroy();
	VBO.destroy();
	EBO.destroy();
	lightVAO.destroy();
	objectVAO.destroy();
	objectShader.destroy();
	lightShader.destroy();
	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////////


void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS) {
		if (xpos == 0.0f) {
			xpos += static_cast<double>(WINDOW_WIDTH - 1);
			camera.lastX += (WINDOW_WIDTH - 1.0f);
			glfwSetCursorPos(window, xpos, ypos);
		}
		else if (xpos + 0.1 >= WINDOW_WIDTH) {
			xpos -= static_cast<double>(WINDOW_WIDTH - 1);
			camera.lastX -= (WINDOW_WIDTH - 1.0f);
			glfwSetCursorPos(window, xpos, ypos);
		}

		float x = static_cast<float>(xpos);
		float y = static_cast<float>(ypos);

		float xoffset = x - camera.lastX;
		float yoffset = camera.lastY - y;

		camera.lastX = x;
		camera.lastY = y;

		camera.processMouseMovement(xoffset, yoffset);
	}
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			camera.processKeyboard(Camera::Movement::FORWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			camera.processKeyboard(Camera::Movement::BACKWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			camera.processKeyboard(Camera::Movement::LEFT, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			camera.processKeyboard(Camera::Movement::RIGHT, deltaTime);
		}
	}
}

void framebufferCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.processMouseScroll((float) yoffset);
}