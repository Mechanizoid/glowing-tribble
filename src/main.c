/* learning OpenGL with C */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>
#include "shaders.h"

/* macros */
#define SCR_WIDTH  800
#define SCR_HEIGHT 600

/* function declarations */
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

int main(void)
{
	unsigned int shaders[2];

	/* === Open window and initialize OpenGL === */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* create window object */
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
					      "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	fprintf(stdout, "Status: created window\n");

	/* initialize GLEW */
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		/* problem, glewInit() failed, something is seriously wrong */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	} else {
		fprintf(stdout, "Status: using GLEW %s\n",
			glewGetString(GLEW_VERSION));
	}

	/* set viewport */
	glViewport(0, 0, 800, 600);

	/* set framebuffer size callback before entering render loop */
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	/* === Set Up Shader Programs === */
	shaders[0] = load_shader_prog("./src/shaders/vertex.glsl",
				      "./src/shaders/frag_orange.glsl");
	shaders[1] = load_shader_prog("./src/shaders/vertex.glsl",
				      "./src/shaders/frag_yellow.glsl");


	/* === Store Elements in Buffers === */

	/* vertex array */
	float triangle1[] = {
		// first triangle
		-0.75f, -0.25f, 0.0f,   // bottom left
		-0.5f, 0.25f, 0.0f,     // top
		-0.25f, -0.25f, 0.0f,   // bottom right
	};

	float triangle2[] = {
		// second triangle
		0.25f, -0.25f, 0.0f,    // bottom left
		0.5f, 0.25f, 0.0f,      // top
		0.75f, -0.25f, 0.0f,    // bottom right
	};

	/* Create vertex array object to hold our vertex buffer and index
	 * buffer objects, then initialize them and store vertices and
	 * indices in their respective objects.
	 */

	unsigned int vaos[2], vbos[2];

	glGenVertexArrays(2, vaos);
	glGenBuffers(2, vbos);

	// triangle 1
	glBindVertexArray(vaos[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1,
		     GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			      (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);

	// triangle 2
	glBindVertexArray(vaos[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2,
		     GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			      (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	/* ==== Render Loop ==== */
	while(!glfwWindowShouldClose(window)) {
		/* get key presses from user */
		process_input(window);

		/* set background color */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/* draw triangles */
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glUseProgram(shaders[0]);
		glBindVertexArray(vaos[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glUseProgram(shaders[1]);

		glBindVertexArray(vaos[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		/* swap buffers */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

/* helps resize the OpenGL viewport when the user resizes the window */
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/* listens to input from the user */
void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
