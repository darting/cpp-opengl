#include "OpenGL3Context.h"




OpenGL3Context::OpenGL3Context() :
_ready(false),
_errorsEnabled(true),
_currentDepthMask(true),
_currentProgram(0),
_viewportX(0),
_viewportY(0),
_viewportWidth(0),
_viewportHeight(0),
_currentIndexBuffer(0),
_currentVertexBuffer(8, 0) {

#if defined _WIN32
	_ready = glewInit() == GLEW_OK;
#else
	_ready = true;
#endif

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	const char* glVendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	const char* glRenderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	const char* glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

	std::cout << "Vender: " << glVendor << std::endl;
	std::cout << "Renderer: " << glRenderer << std::endl;
	std::cout << "Version: " << glVersion << std::endl;
}

OpenGL3Context::~OpenGL3Context() {
	for (auto& vertexBuffer : _vertexBuffers) {
		glDeleteBuffers(1, &vertexBuffer);
	}
	for (auto& indexBuffer : _indexBuffers) {
		glDeleteBuffers(1, &indexBuffer);
	}
	for (auto& program : _programs) {
		glDeleteProgram(program);
	}
	for (auto& vertexShader : _vertexShaders) {
		glDeleteShader(vertexShader);
	}
	for (auto& fragmentShader : _fragmentShaders) {
		glDeleteShader(fragmentShader);
	}
}

void  OpenGL3Context::configureViewport(const uint x, const uint y, const uint width, const uint height) {
	if (x != _viewportX || y != _viewportY || width != _viewportWidth || height != _viewportHeight) {
		_viewportX = x;
		_viewportY = y;
		_viewportWidth = width;
		_viewportHeight = height;
		glViewport(x, y, width, height);
	}
}

void OpenGL3Context::clear(float red, float green, float blue, float alpah, float depth, uint stencil, uint mask) {
	glClearColor(red, green, blue, alpah);
	// TODO check gl version ?
	glClearDepth(depth);
	mask = (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) & mask;
	if (mask & GL_DEPTH_BUFFER_BIT) {
		glDepthMask(_currentDepthMask = true);
	}
	glClear(mask);
}

void OpenGL3Context::present() {
	// TODO
}

void OpenGL3Context::drawTriangles(const uint indexBuffer, const int numTriangles) {
	if (_currentIndexBuffer != indexBuffer) {
		_currentIndexBuffer = indexBuffer;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	}
	glDrawElements(GL_TRIANGLES, numTriangles * 3, GL_UNSIGNED_SHORT, (void*)0);
	checkForErrors();
}

const uint OpenGL3Context::createVertexBuffer(const uint size) {
	uint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), 0, GL_STATIC_DRAW);
	_vertexBuffers.push_back(vertexBuffer);
	checkForErrors();
	return vertexBuffer;
}

void OpenGL3Context::uploadVertexBufferData(const uint vertexBuffer, const uint offset, const uint size, void* data) {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(GLfloat), size * sizeof(GLfloat), data);
	checkForErrors();
}

void OpenGL3Context::deleteVertexBuffer(const uint vertexBuffer) {
	for (auto& currentVertexBuffer : _currentVertexBuffer) {
		if (currentVertexBuffer == vertexBuffer)
			currentVertexBuffer = 0;
	}
	_vertexBuffers.erase(std::find(_vertexBuffers.begin(), _vertexBuffers.end(), vertexBuffer));
	glDeleteBuffers(1, &vertexBuffer);
	checkForErrors();
}

void OpenGL3Context::setVertexBufferAt(const uint position, const uint vertexBuffer, const uint size, const uint stride, const uint offset) {
	auto currentVertexBuffer = _currentVertexBuffer[position];
	if (currentVertexBuffer == vertexBuffer
		&& _currentVertexSize[position] == size
		&& _currentVertexStride[position] == stride
		&& _currentVertexOffset[position] == position)
		return;

	_currentVertexBuffer[position] == vertexBuffer;
	_currentVertexSize[position] = size;
	_currentVertexStride[position] = stride;
	_currentVertexOffset[position] = position;

	if (vertexBuffer <= 0) {
		glDisableVertexAttribArray(position);
		return;
	}

	glEnableVertexAttribArray(position);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(position, size, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * stride, (void*)(sizeof(GLfloat) * offset));

	checkForErrors();
}

const uint OpenGL3Context::createIndexBuffer(const uint size) {
	uint indexBuffer;

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	_currentIndexBuffer = indexBuffer;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLushort), 0, GL_STATIC_DRAW);
	
	_indexBuffers.push_back(indexBuffer);

	checkForErrors();

	return indexBuffer;
}

void OpenGL3Context::uploadIndexBufferData(const uint indexBuffer, const uint offset, const uint size, void* data) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	_currentIndexBuffer = indexBuffer;
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(GLushort), size * sizeof(GLushort), data);
	checkForErrors();
}

void OpenGL3Context::deleteIndexBuffer(const uint indexBuffer) {
	if (_currentIndexBuffer == indexBuffer) {
		_currentIndexBuffer = 0;
	}
	_indexBuffers.erase(std::find(_indexBuffers.begin(), _indexBuffers.end(), indexBuffer));
	glDeleteBuffers(1, &indexBuffer);
	checkForErrors();
}

const uint OpenGL3Context::createProgram() {
	auto handle = glCreateProgram();
	checkForErrors();
	_programs.push_back(handle);
	return handle;
}

void OpenGL3Context::attachShader(const uint program, const uint shader) {
	glAttachShader(program, shader);
	checkForErrors();
}

void OpenGL3Context::linkProgram(const uint program) {
	glLinkProgram(program);
#ifdef DEBUG
	auto errors = getProgramInfoLogs(program);
	if(!errors.empty()) {
		std::cout << errors << std::endl;
	}
#endif
	checkForErrors();
}

void OpenGL3Context::deleteProgram(const uint program) {
	_programs.erase(std::find(_programs.begin(), _programs.end(), program));
	glDeleteProgram(program);
	checkForErrors();
}

void OpenGL3Context::compileShader(const uint shader) {
	glCompileShader(shader);

#ifdef DEBUG
	auto errors = getShaderCompilationLogs(shader);
	if (!errors.empty()) {
		std::stringstream stream;
		stream << "glShaderSource_" << shader << ".txt";
	}
#endif // DEBUG
	checkForErrors();
}

void OpenGL3Context::setProgram(const uint program) {
	if (_currentProgram == program) return;

	_currentProgram = program;
	glUseProgram(program);
	checkForErrors();
}

void OpenGL3Context::setShaderSource(const uint shader, const string& source) {
	const char* sourceString = source.c_str();
	glShaderSource(shader, 1, &sourceString, 0);
	checkForErrors();
}

const uint OpenGL3Context::createVertexShader() {
	uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	_vertexShaders.push_back(vertexShader);
	checkForErrors();
	return vertexShader;
}

void OpenGL3Context::deleteVertexBuffer(const uint vertexShader) {
	_vertexShaders.erase(std::find(_vertexShaders.begin(), _vertexShaders.end(), vertexShader));
	glDeleteShader(vertexShader);
	checkForErrors();
}

const uint OpenGL3Context::createFragmentShader() {
	uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	_fragmentShaders.push_back(fragmentShader);
	checkForErrors();
	return fragmentShader;
}

void OpenGL3Context::deleteFragmentShader(const uint fragmentShader) {
	_fragmentShaders.erase(std::find(_fragmentShaders.begin(), _fragmentShaders.end(), fragmentShader));
	glDeleteShader(fragmentShader);
	checkForErrors();
}

std::shared_ptr<ProgramInputs> OpenGL3Context::getProgramInputs(const uint program) {
	std::vector<string> names;
	std::vector<uint> types;
	std::vector<uint> locations;
	
	setProgram(program);
	fillUniformInputs(program, names, types, locations);
	fillAttributeInputs(program, names, types, locations);

	return ProgramInputs::create(shared_from_this(), program, names, types, locations);
}

void OpenGL3Context::fillUniformInputs(const uint program, std::vector<string>& names, std::vector<uint>& types, std::vector<uint>& locations) {
	int total = -1;
	int maxUniformNameLength = -1;

	glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &total);

	for (int i = 0; i < total; ++i) {
		int nameLength = -1;
		int size = -1;
		GLenum type = GL_ZERO;
		std::vector<char> name(maxUniformNameLength);

		glGetActiveUniform(program, i, maxUniformNameLength, &nameLength, &size, &type, &name[0]);
		checkForErrors();

		name[nameLength] = 0;
		int location = glGetUniformLocation(program, &name[0]);

		if (location >= 0) {
			names.push_back(string(&name[0], nameLength));
			types.push_back(type);
			locations.push_back(location);
		}
	}
}

void OpenGL3Context::fillAttributeInputs(const uint program, std::vector<string>& names, std::vector<uint>& types, std::vector<uint>& locations) {
	int total = -1;
	int maxAttributeNameLength = -1;

	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeNameLength);
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &total);

	for (int i = 0; i < total; ++i) {
		int nameLength = -1;
		int size = -1;
		GLenum type = GL_ZERO;
		std::vector<char> name(maxAttributeNameLength);

		glGetActiveAttrib(program, i, maxAttributeNameLength, &nameLength, &size, &type, &name[0]);
		checkForErrors();

		name[nameLength] = 0;
		int location = glGetAttribLocation(program, &name[0]);
		if (location >= 0) {
			names.push_back(string(&name[0], nameLength));
			types.push_back(type);
			locations.push_back(location);
		}
	}
}

string OpenGL3Context::getShaderCompilationLogs(const uint shader) {
	int compileStatus = -1;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE) {
		int logsLength = -1;
		char buffer[1024];
		int bufferLength = -1;
		glGetShaderSource(shader, 1024, &bufferLength, buffer);
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logsLength);
		if (logsLength > 0) {
			std::vector<char> logs(logsLength);
			glGetShaderInfoLog(shader, logsLength, &logsLength, &logs[0]);
			return std::string(&logs[0]);
		}
	}
	return std::string();
}

string OpenGL3Context::getProgramInfoLogs(const uint program) {
	int programInfoMaxLength = -1;
	int programInfoLength = -1;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &programInfoMaxLength);
	if (programInfoMaxLength <= 0)
		return string();
	std::vector<char> programInfo(programInfoMaxLength);
	glGetProgramInfoLog(program, programInfoMaxLength, &programInfoLength, &programInfo[0]);
	return string(&programInfo[0]);
}

uint OpenGL3Context::getError() {
	auto error = glGetError();
	switch (error) {
	default:
		break;
	case GL_INVALID_ENUM:
		std::cerr << "GL_INVALID_ENUM" << std::endl;
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
		break;
	case GL_INVALID_VALUE:
		std::cerr << "GL_INVALID_VALUE" << std::endl;
		break;
	case GL_INVALID_OPERATION:
		std::cerr << "GL_INVALID_OPERATION" << std::endl;
		break;
	case GL_OUT_OF_MEMORY:
		std::cerr << "GL_OUT_OF_MEMORY" << std::endl;
		break;
	}
	return error;
}

inline void OpenGL3Context::checkForErrors() {
#ifdef DEBUG
	if (_errorsEnabled && getError() != 0) {
		std::cout << "error: OpenGL3Context::checkForErrors()" << std::endl;
	}
#endif // DEBUG
}