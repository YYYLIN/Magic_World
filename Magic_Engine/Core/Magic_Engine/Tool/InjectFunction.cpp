#include "InjectFunction.h"

std::map<std::string, std::vector<std::function<void(void)>>> S_map_inject_function;

std::vector<int> S_TEST;

void Run_Inject_function(const char* _C) {
	const auto _EngineRunStartFunction = S_map_inject_function.find(_C);
	if (_EngineRunStartFunction != S_map_inject_function.end()) {
		for (const auto& _Fun : _EngineRunStartFunction->second) {
			_Fun();
		}
	}
}