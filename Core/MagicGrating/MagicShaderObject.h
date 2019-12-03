#ifndef _MAGICSHADEROBJECT_H_
#define _MAGICSHADEROBJECT_H_


#include "MagicStruct.h"
#include "GrowableArray.hpp"

class MagicShaderObject
{
public:
	MagicShaderObject();
	~MagicShaderObject();

	bool Initialize(const MG_VertexShader& _pMG_VertexShader, const MG_PixelShader& _pMG_PixelShader);
	void Shutdown();

	unsigned int CreateVariate(const unsigned int& _size);

	void MG_ShaderVariate(const unsigned int& _Pos, const void* _pValue);

	void* GetNEW_NowVariate();

	inline MG_VertexShader GetMG_VertexShader() { return pMG_VertexShader; }
	inline MG_PixelShader GetMG_PixelShader() { return pMG_PixelShader; }
private:
	void* m_NowMG_VertexVariate;
    CGrowableArray<unsigned int> G_PosVariate;

	MG_VertexShader pMG_VertexShader;
	MG_PixelShader pMG_PixelShader;
};

#endif