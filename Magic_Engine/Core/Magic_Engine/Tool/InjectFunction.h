#ifndef _AUTO_LOAD_FUNCTION_H_
#define _AUTO_LOAD_FUNCTION_H_

#include <functional> 


void Inject_function(const char* _C, const std::function<void(void)>& _F, bool _Run = false);

#define _INJECT_FUNCTION_(N, C, F)	static int S_INJECT_FUNCTION_##N = [](){  \
										Inject_function(C, F); \
										return 0; \
									}()

/*
ע�뺯��,������cppλ�õ��ñ�д��F���������ڵ���MAINǰע�뵽C�¼���ȥ
*/
#define __INJECT_FUNCTION__(N, C, F)	_INJECT_FUNCTION_(N, C, F)
#define INJECT_FUNCTION(C, F)	__INJECT_FUNCTION__(__COUNTER__, C, F)

#endif