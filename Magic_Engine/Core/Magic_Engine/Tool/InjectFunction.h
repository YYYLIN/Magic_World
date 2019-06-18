#ifndef _AUTO_LOAD_FUNCTION_H_
#define _AUTO_LOAD_FUNCTION_H_

#include <map>
#include <vector>
#include <functional> 
#include <string>

static std::map<std::string, std::vector<std::function<void(void)>>> S_map_inject_function;

#define _INJECT_FUNCTION_(N, C, F)	static int S_INJECT_FUNCTION_##N = [](){  \
										S_map_inject_function[C].push_back(F); \
										return 0; \
									}()
			
/*
注入函数,在任意cpp位置调用编写的F函数都将在调用MAIN前注入到C事件中去
*/
#define INJECT_FUNCTION(C, F)	_INJECT_FUNCTION_(__COUNTER__, C, F)

#endif