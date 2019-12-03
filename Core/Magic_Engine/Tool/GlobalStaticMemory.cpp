#include "GlobalStaticMemory.h"
#include <vector>

#define GLOBALSTATIC_NEW_MIN_MEMORY		1024*1024		//申请一个全家静态内存的对象，所需要创建新的区域块的最小内存
#define GLOBALSTATIC_NEW_MAX_MEMORY		1024*1024*100	//申请一个全家静态内存的对象，所需要创建新的区域块的最大内存
#define GLOBALSTATIC_NEW_MAX_NUMBER		10

struct GS_MemoryPiece
{
	unsigned char* pMemory;
	size_t Size;
	size_t EndPos;
};

static std::vector<GS_MemoryPiece> G_vec_GS_MemoryPiece;

void* NewGlobalStaticMemory(const size_t& _size) {
	for (auto& _auto : G_vec_GS_MemoryPiece) {
		if (_auto.Size - _auto.EndPos >= _size) {
			void* _result = _auto.pMemory + _auto.EndPos;
			_auto.EndPos += _size;
			return _result;
		}
	}

	GS_MemoryPiece _GS_MemoryPiece;

	size_t _NumberSize = GLOBALSTATIC_NEW_MAX_NUMBER * _size;
	if (_NumberSize < GLOBALSTATIC_NEW_MIN_MEMORY) {
		_NumberSize = GLOBALSTATIC_NEW_MIN_MEMORY;
	}
	else if (_NumberSize > GLOBALSTATIC_NEW_MAX_MEMORY) {
		if (_size < GLOBALSTATIC_NEW_MAX_MEMORY) {
			_NumberSize = GLOBALSTATIC_NEW_MAX_MEMORY;
		}
		else {
			_NumberSize = _size;
		}
	}
	_GS_MemoryPiece.Size = _NumberSize;
	_GS_MemoryPiece.pMemory = (unsigned char*)malloc(_NumberSize);
	_GS_MemoryPiece.EndPos = _size;

	G_vec_GS_MemoryPiece.push_back(_GS_MemoryPiece);

	return _GS_MemoryPiece.pMemory;
}

void DeleteGSAll() {
	for (auto& _auto : G_vec_GS_MemoryPiece) {
		free(_auto.pMemory);
	}
	G_vec_GS_MemoryPiece.clear();
}