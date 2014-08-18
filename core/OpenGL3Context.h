#pragma once

#include "Common.h"
#include "ProgramInputs.h"


class OpenGL3Context :
	public std::enable_shared_from_this <OpenGL3Context> {
public:
	typedef std::shared_ptr<OpenGL3Context> Ptr;
	~OpenGL3Context();

	static Ptr create() {
		return std::shared_ptr<OpenGL3Context>(new OpenGL3Context());
	}

	void configureViewport(const uint x, const uint y, const uint width, const uint height);

	void clear(float red = 0.f,
		float green = 0.f,
		float blue = 0.f,
		float alpha = 0.f,
		float depth = 1.f,
		unsigned int stencil = 0,
		unsigned int mask = 0xffffffff);

	void present();

	void drawTriangles(const uint indexBuffer, const int numTriangles);

	const uint createVertexBuffer(const uint size);

	void setVertexBufferAt(const uint position,
		const uint vertexBuffer,
		const uint size,
		const uint stride,
		const uint offset);

	void uploadVertexBufferData(const uint vertexBuffer,
		const uint offset,
		const uint size,
		void* data);

	void deleteVertexBuffer(const uint vertexBuffer);

	const uint createIndexBuffer(const uint size);

	void uploadIndexBufferData(const uint indexBuffer,
		const uint offset,
		const uint size,
		void* data);

	void deleteIndexBuffer(const uint indexBuffer);

	const uint createProgram();

	void attachShader(const uint program, const uint shader);

	void linkProgram(const uint program);

	void deleteProgram(const uint program);

	void compileShader(const uint shader);

	void setProgram(const uint program);

	void setShaderSource(const uint shadr, const string& source);

	const uint createVertexShader();

	void deleteVertexShader();

	void deleteVertexShader(const uint vertexShader);

	const uint createFragmentShader();

	void deleteFragmentShader(const uint fragmentShader);


	std::shared_ptr<ProgramInputs> getProgramInputs(const uint program);



private:
	bool _errorsEnabled = true;
};

