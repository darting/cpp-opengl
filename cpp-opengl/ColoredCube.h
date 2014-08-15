#pragma once

#include "Common.h"
#include "Scene.h"

class ColoredCube: public Scene
{
public:
	ColoredCube();
	~ColoredCube();

	void prepare();
	void bind();
	void draw();
	void cleanup();
	void dispose();

private:

	GLuint vertexBuffer;
	std::vector<GLfloat> vertexBufferData;

	GLuint colorBuffer;
	std::vector<GLfloat> colorBufferData;

};

