#ifndef _MAGIC_OPENGL_H_
#define _MAGIC_OPENGL_H_

namespace Magic
{
	typedef  struct {
		unsigned int count;
		unsigned int instanceCount;
		unsigned int first;
		unsigned int baseInstance;
	} DrawArraysIndirectCommand;

	typedef struct {
		unsigned int count;
		unsigned int instanceCount;
		unsigned int firstIndex;
		unsigned int baseVertex;
		unsigned int baseInstance;
	} DrawElementsIndirectCommand;
}

#endif