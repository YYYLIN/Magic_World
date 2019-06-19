#ifndef _AUTO_LOAD_FUNCTION_H_
#define _AUTO_LOAD_FUNCTION_H_

#include <map>
#include <vector>
#include <functional> 
#include <string>

extern std::map<std::string, std::vector<std::function<void(void)>>> S_map_inject_function;

extern std::vector<int> S_TEST;

void Run_Inject_function(const char* _C);

#define _INJECT_FUNCTION_(N, C, F)	static int S_INJECT_FUNCTION_##N = [](){  \
										S_map_inject_function[C].push_back(F); \
										return 0; \
									}()
			
/*
注入函数,在任意cpp位置调用编写的F函数都将在调用MAIN前注入到C事件中去
*/
#define __INJECT_FUNCTION__(N, C, F)	_INJECT_FUNCTION_(N, C, F)
#define INJECT_FUNCTION(C, F)	__INJECT_FUNCTION__(__COUNTER__, C, F)

#endif