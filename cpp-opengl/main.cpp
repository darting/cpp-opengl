#pragma once

#include "Common.h"
#include "Scene.h"
#include "ColoredCube.h"
#include "Shader.h"
#include "OpenGL3Context.h"



int main(void) {

	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW \n");
		return -1;
	}

	ColoredCube scene;
	scene.prepare();

	Shader shader;
	shader.loadShader("colored.v.glsl", "colored.f.glsl");
	if (!shader.ready()) {
		fprintf(stderr, "Failed to load shader \n");
	}

	mat4 model(1.0f);
	mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	mat4 view = glm::lookAt(vec3(4, 3, -3), vec3(0, 0, 0), vec3(0, 1, 0));
	mat4 mvp = projection * view * model;

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		
		shader.unifrom("MVP", mvp);

		scene.bind();
		scene.draw();
		scene.cleanup();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	scene.dispose();
	shader.dispose();

	glfwTerminate();


	return 0;
}





