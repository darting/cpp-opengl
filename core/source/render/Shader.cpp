#include "render/Shader.h"


using namespace orange;
using namespace orange::render;

Shader::Shader(std::shared_ptr<OpenGL3Context> context, string path) :
_ready(false),
_program(0),
_vertexShader(0),
_fragmentShader(0) {

	_context = context;
	string vertexSource = load(path + ".v.glsl");
	if (vertexSource.empty()) {
		return;
	}

	string fragmentSource = load(path + ".f.glsl");
	if (fragmentSource.empty()) {
		return;
	}

	_ready = compile(vertexSource, fragmentSource);
}


Shader::~Shader() {
	if (_ready) {
		
	}
}


string Shader::load(const string& path) {
	string source;
	std::ifstream vertexStream(path, std::ios::in);
	if (vertexStream.is_open()) {
		string line = "";
		while (getline(vertexStream, line)) {
			source += "\n" + line;
		}
		vertexStream.close();
	} else {
		std::cout << "couldn't load source: " << path << std::endl;
	}
	return source;
}


bool Shader::compile(string& vertexSource, string& fragmentSource) {
	_program = _context->createProgram();

	_vertexShader = _context->createVertexShader();
	_context->setShaderSource(_vertexShader, vertexSource);
	_context->compileShader(_vertexShader);

	_fragmentShader = _context->createFragmentShader();
	_context->setShaderSource(_fragmentShader, fragmentSource);
	_context->compileShader(_fragmentShader);

	_context->attachShader(_program, _vertexShader);
	_context->attachShader(_program, _fragmentShader);
	_context->linkProgram(_program);

	_inputs = _context->getProgramInputs(_program);

	/*int ready = GL_FALSE;
	int infoLogLength;

	std::cout << "Compiling vertex shader \n";

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	char const * vertexSourcePointer = vertexSource.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
	glCompileShader(vertexShaderID);
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &ready);
	if (ready != GL_TRUE) {
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

	
	std::cout << "Compiling fragment shader \n";

	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	char const * fragmentSourcePointer = fragmentSource.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
	glCompileShader(fragmentShaderID);
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &ready);
	if (ready != GL_TRUE) {
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

	std::cout << "Linking program\n";
	_program = glCreateProgram();
	glAttachShader(_program, vertexShaderID);
	glAttachShader(_program, fragmentShaderID);
	glLinkProgram(_program);

	glGetProgramiv(_program, GL_LINK_STATUS, &ready);
	if (ready != GL_TRUE) {
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			std::vector<char> logs(infoLogLength + 1);
			glGetProgramInfoLog(_program, infoLogLength, NULL, &logs[0]);
			std::cout << &logs[0] << std::endl;
		}
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		return false;
	}

	_vertexShader = vertexShaderID;
	_fragmentShader = fragmentShaderID;*/

	return true;
}


void Shader::setUniform(const string& name, const mat4& mat) {
	if (!_inputs->hasName(name))
		return;
	_context->setUniform(_inputs->location(name), mat);
}