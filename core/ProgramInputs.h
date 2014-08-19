#pragma once

#include "Common.h"

namespace orange {

	namespace render {

		class ProgramInputs {
		public:
			typedef std::shared_ptr<ProgramInputs> Ptr;

			inline bool hasName(const string& name) const {
				return _nameToType.find(name) != _nameToType.end();
			}

			inline const std::vector<string> & names() const {
				return _names;
			}

			inline const std::vector<uint>& types() const {
				return _types;
			}

			inline const std::vector<uint>& locations() const {
				return _locations;
			}

			inline const int location(const string& name) const {
				auto foundLocationIt = _nameToLocations.find(name);
				return foundLocationIt != _nameToLocations.end() ? (int)foundLocationIt->second : -1;
			}

			inline const uint type(const string& name) const {
				auto foundTypeIt = _nameToType.find(name);
				return foundTypeIt != _nameToType.end() ? foundTypeIt->second : 0;
			}

		public:
			static Ptr create(std::shared_ptr<OpenGL3Context> context,
				const uint program,
				const std::vector<string>& names,
				const std::vector<uint>& types,
				const std::vector<uint>& locations) {
				return std::shared_ptr<ProgramInputs>(new ProgramInputs(context,
					program,
					names,
					types,
					locations));
			}

		private:
			std::shared_ptr<OpenGL3Context> _context;
			const uint _program;
			std::vector<string> _names;
			std::vector<uint> _types;
			std::vector<uint> _locations;
			std::unordered_map<string, uint> _nameToType;
			std::unordered_map<string, uint> _nameToLocations;

			ProgramInputs(std::shared_ptr<OpenGL3Context> context,
				const uint program,
				const std::vector<string>& names,
				const std::vector<uint>& types,
				const std::vector<uint>& locations) :
				_context(context),
				_program(program),
				_names(names),
				_types(types),
				_locations(locations),
				_nameToType(),
				_nameToLocations() {
#ifdef DEBUG
				if(_types.size() != _names.size() || _locations.size() != _names.size())
					throw;
#endif // DEBUG

				for (uint i = 0; i < _names.size(); ++i) {
					const string& name = _names[i];
					_nameToType[name] = _types[i];
					_nameToLocations[name] = _locations[i];
				}
			}
		};

	}

}