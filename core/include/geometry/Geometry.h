#pragma once

#include "Common.h"

namespace orange{
	namespace geometry {

		class Geometry :
			public std::enable_shared_from_this<Geometry>
		{
		public:
			typedef std::shared_ptr<Geometry> Ptr;

			Geometry();
			~Geometry();
		};

	}
}
