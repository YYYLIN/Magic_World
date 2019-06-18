#ifndef _AUTO_LOAD_FUNCTION_H_
#define _AUTO_LOAD_FUNCTION_H_

#include <map>
#include <vector>
#include <functional> 

static std::map<std::string, std::vector<std::function<void(void)>>> S_map_auto_function;

#define _LOAD_FUNCTION_(N, C, F)	static int LOAD_FUNCTION_##N = [](){\
										auto _auto = S_map_auto_function.find(#C); \
										if (_auto != S_map_auto_function.end()) {  \
											_auto->second.push_back(F); \
										}		\
										else {	\
											S_map_auto_function.insert(std::make_pair(#C, F)); \
										}		\
										return 0; \
									}
			

#define LOAD_FUNCTION(C, F)	_LOAD_FUNCTION_(__COUNTER__, C, F)

#endif