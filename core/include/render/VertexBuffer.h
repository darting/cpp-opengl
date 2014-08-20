#pragma once


#include "Common.h"

namespace orange {
	namespace render {
		class VertexBuffer :
			public std::enable_shared_from_this < VertexBuffer > {
		public:
			typedef std::shared_ptr<VertexBuffer> Ptr;
			typedef const std::tuple<string, uint, uint> Attribute;
			typedef std::shared_ptr<Attribute> AttributePtr;
			typedef std::shared_ptr<vec3> Vector3Ptr;

		public:

			~VertexBuffer();

			inline static Ptr create(std::shared_ptr<OpenGL3Context> context) {
				return std::shared_ptr<VertexBuffer>(new VertexBuffer(context));
			}

			inline static Ptr create(std::shared_ptr<OpenGL3Context> context,
				float* data,
				const uint size,
				const uint offset = 0) {
				return std::shared_ptr<VertexBuffer>(new VertexBuffer(context, data, offset, size));
			}

			inline static Ptr create(std::shared_ptr<OpenGL3Context> context,
				std::vector<float>::const_iterator begin,
				std::vector<float>::const_iterator end) {
				return std::shared_ptr<VertexBuffer>(new VertexBuffer(context, begin, end));
			}

			inline static Ptr create(std::shared_ptr<OpenGL3Context> context, float* begin, float* end) {
				return std::shared_ptr<VertexBuffer>(new VertexBuffer(context, begin, end));
			}

			inline static Ptr create(std::shared_ptr<OpenGL3Context> context, const std::vector<float>& data) {
				return create(context, data.begin(), data.end());
			}

			inline std::vector<float>& data() {
				return _data;
			}

			inline std::list<AttributePtr>& attributes() {
				return _attributes;
			}

			inline const uint vertexSize() {
				return _vertexSize;
			}

			inline uint numVertices() const {
				return _vertexSize > 0 ? _data.size() / _vertexSize : 0;
			}

			inline void upload() {
				upload(0, 0);
			}

			void upload(uint offset, uint numVertices = 0);

			void dispose();

			void addAttribute(const string& name, const uint size, const uint offset = 0);

			void removeAttribute(const string& name);

			bool hasAttribute(const string& attributeName) const;

			AttributePtr attribute(const string& attributeName) const;

			bool equals(std::shared_ptr<VertexBuffer> vertexBuffer) {
				return _data == vertexBuffer->_data;
			}

			Vector3Ptr minPosition(Vector3Ptr output = nullptr);

			Vector3Ptr maxPosition(Vector3Ptr output = nullptr);

			Vector3Ptr centerPosition(Vector3Ptr output = nullptr);

		protected:
			VertexBuffer(std::shared_ptr<OpenGL3Context> context);

			VertexBuffer(std::shared_ptr<OpenGL3Context> context,
				float* data,
				const uint size,
				const uint offset = 0);

			VertexBuffer(std::shared_ptr<OpenGL3Context> context,
				std::vector<float>::const_iterator begin,
				std::vector<float>::const_iterator end);

			VertexBuffer(std::shared_ptr<OpenGL3Context> context, float* begin, float* end);

		private:
			std::vector<float> _data;
			std::list<AttributePtr> _attributes;
			uint _vertexSize;
			Vector3Ptr _minPosition;
			Vector3Ptr _maxPosition;

		};
	}
}




