/* load shaders from seperate GLSL source files */

#include <GL/glew.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "shaders.h"

/* global type definitions */
enum Type{Vertex, Fragment};

/* function declarations */
static char *load_shader_src(char *filename);
static unsigned int compile_shader(enum Type t, char *src);


/* Load GLSL vertex and fragment shader source from files indicated,
 * compile and link them, and return the shader program.
 */
unsigned int load_shader_prog(char *v_src_file, char *f_src_file)
{
	unsigned int vertex_shader, fragment_shader, shader_prog;
	char *src;
	int link_success;
	char infoLog[512];

	/* vertex shader */
	src = load_shader_src(v_src_file);
	vertex_shader = compile_shader(Vertex, src);

	/* frag shader */
	free(src);
	src = load_shader_src(f_src_file);
	fragment_shader = compile_shader(Fragment, src);

	/* link shader program */
	shader_prog = glCreateProgram();
	glAttachShader(shader_prog, vertex_shader);
	glAttachShader(shader_prog, fragment_shader);
	glLinkProgram(shader_prog);

	/* check link status */
	glGetProgramiv(shader_prog, GL_LINK_STATUS, &link_success);
	if (!link_success) {
		glGetProgramInfoLog(shader_prog, 512, NULL, infoLog);
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return shader_prog;
}


/* Compiles vertex or fragment shader from provided source code, then
 * checks for compile errors. If an error is found, it is logged and
 * then the program exits. If compilation is successful, returns the
 * integer handle to the newly compiled shader.
 */
static unsigned int compile_shader(enum Type t, char *src)
{
	unsigned int shader;
	int success;
	char info_log[512];
	char *type_s;

	if (Vertex == t) {
		shader = glCreateShader(GL_VERTEX_SHADER);
		type_s = "VERTEX";
	} else if (Fragment == t) {
		shader = glCreateShader(GL_FRAGMENT_SHADER);
		type_s = "FRAGMENT";
	}

	glShaderSource(shader, 1, (const char* const *) &src, NULL);
	glCompileShader(shader);

	/* check for compile errors */
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, info_log);
		fprintf(stderr, "ERROR::SHADER::%s::COMPILATION::FAILED\n%s\n",
			type_s, info_log);
		exit(EXIT_FAILURE);
	}
	
	return shader;	
}

/* Attempts to open the file of GLSL source code named in the char*
 * parameter. If it is successful, the function returns a pointer
 * to a buffer containing the requested shader source.
 */
static char *load_shader_src(char *filename)
{
	FILE *fp;
	char *src;
	size_t size;

	fp = fopen(filename, "r");
	if (NULL == fp) {
		fprintf(stderr, "ERROR: failed to open file %s\n", filename);
		exit(EXIT_FAILURE);
	}

	fprintf(stdout, "Opened source file: %s\n", filename);
	// find size of file
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	src = calloc(size, sizeof(char));
	if (NULL == src) {
		fprintf(stderr,
			"ERROR: call of calloc() failed in load_shader_src()\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < size; i++) {
		src[i] = fgetc(fp);
	}

	return src;
}
