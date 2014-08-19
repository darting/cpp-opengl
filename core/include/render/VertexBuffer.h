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

			VertexBuffer(std::shared_ptr<OpenGL3Context> context);

			~VertexBuffer();

		public:
			inline static Ptr create(std::shared_ptr<OpenGL3Context> context) {
				return std::shared_ptr<VertexBuffer>(new VertexBuffer(context));
			}

		private:
			std::vector<float> _data;
			std::list<AttributePtr> _attributes;
			uint _vertexSize;
			Vector3Ptr _minPosition;
			Vector3Ptr _maxPosition;

		};
	}
}




