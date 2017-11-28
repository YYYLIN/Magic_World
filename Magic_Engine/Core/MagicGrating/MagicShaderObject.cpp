#include "MagicShaderObject.h"



MagicShaderObject::MagicShaderObject()
{
	m_NowMG_VertexVariate = 0;
}


MagicShaderObject::~MagicShaderObject()
{
	Shutdown();
}


bool MagicShaderObject::Initialize(const MG_VertexShader& _pMG_VertexShader, const MG_PixelShader& _pMG_PixelShader)
{
	pMG_VertexShader = _pMG_VertexShader;
	pMG_PixelShader = _pMG_PixelShader;
	m_NowMG_VertexVariate = malloc(G_PosVariate.GetEnd());

	return true;
}

void MagicShaderObject::Shutdown()
{
	if (m_NowMG_VertexVariate)
		free(m_NowMG_VertexVariate);
}

unsigned int MagicShaderObject::CreateVariate(const unsigned int& _size)
{

	if (!G_PosVariate.GetSize())
	{
		G_PosVariate.Add(0);
		G_PosVariate.Add(_size);
	}
	else
	{
		G_PosVariate.Add(G_PosVariate.GetEnd() + _size);
	}
	return G_PosVariate.GetSize() - 2;
}

void MagicShaderObject::MG_ShaderVariate(const unsigned int& _Pos, const void* _pValue)
{
	memcpy((unsigned char*)m_NowMG_VertexVariate + G_PosVariate[_Pos], _pValue, G_PosVariate[_Pos + 1] - G_PosVariate[_Pos]);
}

void* MagicShaderObject::GetNEW_NowVariate()
{
	void* _pvoid = malloc(G_PosVariate.GetEnd());
	memcpy(_pvoid, m_NowMG_VertexVariate, G_PosVariate.GetEnd());
	return _pvoid;
}