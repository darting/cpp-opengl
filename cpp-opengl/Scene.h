#pragma once

#include "Common.h"

class Scene {
public:
	virtual void prepare() = 0;
	virtual void bind() = 0;
	virtual void draw() = 0;
	virtual void cleanup() = 0;
	virtual void dispose() = 0;
};

