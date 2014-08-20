#pragma once

#include "Common.h"


namespace orange {
	namespace render {
		class AbstractResource {

		protected:
			std::shared_ptr<render::OpenGL3Context>	_context;
			int _id;

			AbstractResource(std::shared_ptr<render::OpenGL3Context> context) :
				_context(context),
				_id(-1) {}

			~AbstractResource() {}

		public:
			inline std::shared_ptr<render::OpenGL3Context> context() {
				return _context;
			}

			inline const int id() {
				if (_id == -1) throw;
				return _id;
			}

			inline const bool ready() {
				return _id != -1;
			}

			virtual void dispose() = 0;
			virtual void upload() = 0;
		};

	}
}