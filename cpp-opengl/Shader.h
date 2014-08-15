#pragma once

#include "Common.h"

class Shader {
public:
	Shader();
	~Shader();

	bool loadShader(string vertexPath, string fragmentPath);

	void use() {
		glUseProgram(_program);
	}

	bool ready() {
		return _ready == GL_TRUE;
	}

	void dispose() {
		glDeleteProgram(_program);
	}

	void unifrom(string name, mat4 mat) {
		char const * pointer = name.c_str();
		glUniformMatrix4fv(glGetUniformLocation(_program, pointer), 1, GL_FALSE, &mat[0][0]);
	}

private:
	GLint _ready = GL_FALSE;
	GLuint _program;
};

