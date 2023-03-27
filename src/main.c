/* learning OpenGL with C */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>

/* macros */
#define SCR_WIDTH  800
#define SCR_HEIGHT 600

/* function declarations */
int check_compile_errors(unsigned int shader, const char *type);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);


/* external variables */

/* vertex shader source */
const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\n";
	
/* fragment shader source */
const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n";


int main(void)
{
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
	

	/* compile vertex shader */
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	check_compile_errors(vertexShader, "VERTEX");

	/* compile fragment shader */
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	check_compile_errors(fragmentShader, "FRAGMENT");

	/* link the shader programs */
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	/* check for linking errors */
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

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
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glUseProgram(shaderProgram);
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

/* check for shader compilation errors, if one occurs get info log and return zero
 * */
int check_compile_errors(unsigned int shader, const char *type)
{
	int success = 0;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		fprintf(stderr, "ERROR::SHADER::%s::COMPILATION::FAILED\n%s\n", type, infoLog);
	}
	return success;
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
