#ifndef _MAGICSTRUCT_H_
#define _MAGICSTRUCT_H_

#include "MagicMath.hpp"

#ifdef _WIN64
typedef long long MGObjectType;
#elif _WIN32
typedef unsigned int MGObjectType;
#endif


typedef MGObjectType MGContext;
typedef MGObjectType MGShader;
typedef MGObjectType MGBuffer;
typedef MGObjectType MGVertexArrays;
typedef MGObjectType MGFrameBuffer;


struct MG_ShaderVertexType
{
	unsigned char PointerNumber;

};

//º¯ÊýÖ¸Õë
typedef Magic::vector4(*MG_VertexShader)(
	void* const pVertex,
	void* const pVariate
	);

typedef Magic::vector4(*MG_PixelShader)(
	void* const pIn,
	void* const pVariate
	);

#endif