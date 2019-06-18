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
ע�뺯��,������cppλ�õ��ñ�д��F���������ڵ���MAINǰע�뵽C�¼���ȥ
*/
#define INJECT_FUNCTION(C, F)	_INJECT_FUNCTION_(__COUNTER__, C, F)

#endif