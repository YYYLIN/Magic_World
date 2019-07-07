#ifndef _GLOBALSTATICMEMORY_H_
#define _GLOBALSTATICMEMORY_H_

#include "stdio.h"

void* NewGlobalStaticMemory(const size_t& _size);

template<class T,class... A>
T* NewGS(A... args) {
	void* _pVoid = NewGlobalStaticMemory(sizeof(T));
	::new (_pVoid) T(args);
	return (T*)_pVoid;
}

void DeleteGSAll();

#endif