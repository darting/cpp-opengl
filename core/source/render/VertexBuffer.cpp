#include "render/VertexBuffer.h"
#include "render/OpenGL3Context.h"


using namespace orange;
using namespace orange::render;


static const std::string ATTRNAME_POSITION = "position";


VertexBuffer::VertexBuffer(std::shared_ptr<OpenGL3Context> context) :
AbstractResource(context),
std::enable_shared_from_this<VertexBuffer>(),
_data(),
_vertexSize(0) {
}


VertexBuffer::VertexBuffer(std::shared_ptr<OpenGL3Context> context,
	float* data,
	const uint size,
	const uint offset = 0) :
	AbstractResource(context),
	_data(data + offset, data + offset + size),
	_vertexSize(0) {
	upload();
}


VertexBuffer::VertexBuffer(std::shared_ptr<OpenGL3Context> context,
	std::vector<float>::const_iterator begin,
	std::vector<float>::const_iterator end) :
	AbstractResource(context),
	_data(begin, end),
	_vertexSize(0) {
	upload();
}


VertexBuffer::VertexBuffer(std::shared_ptr<OpenGL3Context> context, float* begin, float* end) :
AbstractResource(context),
_data(begin, end),
_vertexSize(0) {
	upload();
}


VertexBuffer::~VertexBuffer() {
}


void VertexBuffer::upload(uint offset, uint numVertices) {
	if (_id == -1)
		_id = _context->createVertexBuffer(_data.size());

	_context->uploadVertexBufferData(_id, 
		offset * _vertexSize, 
		numVertices == 0 ? _data.size() : numVertices * _vertexSize, 
		&_data[0]);
}


void VertexBuffer::dispose() {
	if (_id != -1) {
		_context->deleteVertexBuffer(_id);
		_id = -1;
	}
	_data.clear();
}


void VertexBuffer::addAttribute(const string& name, const uint size, const uint offset) {
	if (hasAttribute(name))
		throw std::invalid_argument("name");

	_attributes.push_back(VertexBuffer::AttributePtr(new VertexBuffer::Attribute(name, size, offset == 0 ? _vertexSize : offset)));

	vertexSize(_vertexSize + size);

	if (name == ATTRNAME_POSITION)
		invalidatePositionBounds();
}


bool VertexBuffer::hasAttribute(const string& attributeName) const {
	auto it = std::find_if(_attributes.begin(), _attributes.end(), [&](AttributePtr attr) {
		return std::get<0>(*attr) == attributeName;
	});
	return it != _attributes.end();
}

void VertexBuffer::removeAttribute(const string& attributeName) {
	auto it = std::find_if(_attributes.begin(), _attributes.end(), [&](AttributePtr attr) {
		return std::get<0>(*attr) == attributeName;
	});

	if (it == _attributes.end())
		throw std::invalid_argument("attributeName = " + attributeName);

	vertexSize(_vertexSize - std::get<1>(**it));
	_attributes.erase(it);

	if (attributeName == ATTRNAME_POSITION)
		invalidatePositionBounds();
}


VertexBuffer::AttributePtr VertexBuffer::attribute(const string& attributeName) const {
	for (auto& attr : _attributes) {
		if (std::get<0>(*attr) == attributeName)
			return attr;
	}

	throw std::invalid_argument("attributeName = " + attributeName);
}


void VertexBuffer::vertexSize(uint value) {
	int offset = value - _vertexSize;

	_vertexSize = value;
}


