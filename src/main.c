/* learning OpenGL with C */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>

/* function declarations */
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* create window object */
	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

	/* ==== Render Loop ==== */
	while(!glfwWindowShouldClose(window)) {
		/* get key presses from user */
		process_input(window);
		
		/* set background color */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

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
