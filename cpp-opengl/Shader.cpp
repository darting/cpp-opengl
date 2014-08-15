#include "Shader.h"


Shader::Shader() {
	
}


Shader::~Shader() {
}

bool Shader::loadShader(string vertexPath, string fragmentPath) {
	
	string vertexSource;
	std::ifstream vertexStream(vertexPath, std::ios::in);
	if (vertexStream.is_open()) {
		string line = "";
		while (getline(vertexStream, line)) {
			vertexSource += "\n" + line;
		}
		vertexStream.close();
	}

	string fragmentSource;
	std::ifstream fragmentStream(fragmentPath, std::ios::in);
	if (fragmentStream.is_open()) {
		string line = "";
		while (getline(fragmentStream, line)) {
			fragmentSource += "\n" + line;
		}
		fragmentStream.close();
	}

	_ready = GL_FALSE;
	int infoLogLength;

	printf("Compiling vertex shader : %s\n", vertexPath.c_str());
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	char const * vertexSourcePointer = vertexSource.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
	glCompileShader(vertexShaderID);

	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &_ready);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> vertexShaderError(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderError[0]);
		printf("%s\n", &vertexShaderError[0]);
		glDeleteShader(vertexShaderID);
		return false;
	}


	printf("Compiling fragment shader : %s\n", fragmentPath.c_str());
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	char const * fragmentSourcePointer = fragmentSource.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderID);

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &_ready);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> fragmentShaderError(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderError[0]);
		printf("%s\n", &fragmentShaderError[0]);
		glDeleteShader(fragmentShaderID);
		return false;
	}

	printf("Linking program\n");

	_program = glCreateProgram();
	glAttachShader(_program, vertexShaderID);
	glAttachShader(_program, fragmentShaderID);
	glLinkProgram(_program);

	glGetProgramiv(_program, GL_LINK_STATUS, &_ready);
	glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> programError(infoLogLength + 1);
		glGetProgramInfoLog(_program, infoLogLength, NULL, &programError[0]);
		printf("%s\n", &programError[0]);
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	_ready = GL_TRUE;

	return ready();
}

