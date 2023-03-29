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
	unsigned int shader_prog;
	
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
		fprintf(stdout, "Status: using GLEW %s\n", glewGetString(GLEW_VERSION));
	}	

	/* set viewport */
	glViewport(0, 0, 800, 600);
	
	/* set framebuffer size callback, we do this after the window is created but
	 * before we enter the render loop
	 */
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	/* get shader program */
	shader_prog = load_shader_prog("./src/shaders/vertex.glsl",
				       "./src/shaders/fragment.glsl");

	
	/* set up vertices to draw */
	/* vertex array */
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f,  // top left
		0.5f, 0.5f, 0.0f,   // top right
		0.5f, -0.5f, 0.0f   // bottom right
	};

	unsigned int indices[] = {
		0, 1, 2,           // first triangle
		0, 3, 2
	};

	/* create VAO, vbo, & ebo; then bind objects to it */
	unsigned int vao, vbo, ebo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
		     GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		     GL_STATIC_DRAW);
	
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			      (void*)0);
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
		glUseProgram(shader_prog);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
