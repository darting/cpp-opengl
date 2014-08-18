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
	} else {
		std::cout << "couldn't load vertex source.\n";
		return false;
	}

	string fragmentSource;
	std::ifstream fragmentStream(fragmentPath, std::ios::in);
	if (fragmentStream.is_open()) {
		string line = "";
		while (getline(fragmentStream, line)) {
			fragmentSource += "\n" + line;
		}
		fragmentStream.close();
	} else {
		std::cout << "couldn't load fragment source.\n";
		return false;
	}

	_ready = GL_FALSE;
	int infoLogLength;

	printf("Compiling vertex shader : %s\n", vertexPath.c_str());
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	char const * vertexSourcePointer = vertexSource.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
	glCompileShader(vertexShaderID);
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &_ready);
	if (_ready != GL_TRUE) {
		infoLogLength = -1;
		glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			std::vector<char> logs(infoLogLength);
			glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &logs[0]);
			std::cout << &logs[0] << std::endl;
		}
		glDeleteShader(vertexShaderID);
		return false;
	}

	printf("Compiling fragment shader : %s\n", fragmentPath.c_str());
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	char const * fragmentSourcePointer = fragmentSource.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderID);
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &_ready);
	if (_ready != GL_TRUE) {
		infoLogLength = -1;
		glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			std::vector<char> logs(infoLogLength);
			glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &logs[0]);
			std::cout << &logs[0] << std::endl;
		}
		glDeleteShader(fragmentShaderID);
		return false;
	}

	printf("Linking program\n");
	_program = glCreateProgram();
	glAttachShader(_program, vertexShaderID);
	glAttachShader(_program, fragmentShaderID);
	glLinkProgram(_program);

	glGetProgramiv(_program, GL_LINK_STATUS, &_ready);
	if (_ready != GL_TRUE) {
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			std::vector<char> logs(infoLogLength + 1);
			glGetProgramInfoLog(_program, infoLogLength, NULL, &logs[0]);
			printf("%s\n", &logs[0]);
		}
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		return false;
	}

	_ready = GL_TRUE;
	return true;
}

