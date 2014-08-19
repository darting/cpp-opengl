#pragma once

#include "Common.h"
#include "OpenGL3Context.h"


namespace orange {
	namespace render {

		class Shader {
		public:
			typedef std::shared_ptr<Shader> Ptr;
			~Shader();

		public:
			inline static Ptr create(std::shared_ptr<OpenGL3Context> context, string path) {
				return std::shared_ptr<Shader>(new Shader(context, path));
			}

			inline const int program() {
				return _program;
			}

			void setUniform(const string& name, const mat4& mat);

		protected:
			std::shared_ptr<OpenGL3Context> _context;
			bool _ready;

			int _program;
			uint _vertexShader;
			uint _fragmentShader;
			std::shared_ptr<ProgramInputs> _inputs;

			Shader(std::shared_ptr<OpenGL3Context> context, string path);

			string load(const string& path);
			bool compile(string& vertexSource, string& fragmentSource);
		};
	}
}



