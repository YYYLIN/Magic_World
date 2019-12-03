#include "InjectFunction.h"

#include <map>
#include <vector>
#include <string>

void Inject_function(const char* _C, const std::function<void(void)>& _F, bool _Run) {
	static std::map<std::string, std::vector<std::function<void(void)>>> S_map_inject_function;
	if (_Run) {
		const auto _EngineRunStartFunction = S_map_inject_function.find(_C);
		if (_EngineRunStartFunction != S_map_inject_function.end()) {
			for (const auto& _Fun : _EngineRunStartFunction->second) {
				_Fun();
			}
		}
	}
	else {
		S_map_inject_function[_C].push_back(_F);
	}

}