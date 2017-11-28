#include "MagicGratingAPI.h"
#include "MagicGlobalVariable.h"
#include "MagicShaderObject.h"
#include "MagicInsideMacro.h"

bool CreateMagicGratingContext(MGContext* _MGContext)
{
	*_MGContext = reinterpret_cast<MGContext>(new MagicGlobalContext);
	if (!*_MGContext)
		return false;

	return true;
}

bool InitMagicGratingContext(const MGContext& _MGContext)
{
	return reinterpret_cast<MagicGlobalContext*>(_MGContext)->Initialize();
}

void DeleteMagicGratingContext(MGContext* _MGContext)
{
	MagicGlobalContext::pMagicGlobalContext->ShutdownGlobalContext();
	delete reinterpret_cast<MagicGlobalContext*>(*_MGContext);
	*_MGContext = 0;
}

void BindMagicGratingContext(const MGContext& _MGContext)
{
	MagicGlobalContext::pMagicGlobalContext = reinterpret_cast<MagicGlobalContext*>(_MGContext);
}

bool BindMGDisplayContext(const MGContext& _MGContext, void* _data,
	const unsigned int& _width, const unsigned int& _Height, const short& _mode, const unsigned short& _type)
{
	DisplayerData _DisplayerData;
	_DisplayerData.pBuffer = _data;
	_DisplayerData.m_Width = _width;
	_DisplayerData.m_Height = _Height;
	_DisplayerData.m_Mode = _mode;
	_DisplayerData.m_Type = _type;
	reinterpret_cast<MagicGlobalContext*>(_MGContext)->SetDisplayerData(_DisplayerData);

	return true;
}

void MG_WaitSwapFrame()
{
	MagicGlobalContext::pMagicGlobalContext->WaitSwapFrame();
}

void MGDrawArrays(const unsigned int& _DrawMode, const unsigned int& _VertexStart, const unsigned int&  _VertexNumber)
{
	MagicGlobalContext::pMagicGlobalContext->AddDrawObject(_DrawMode, _VertexStart, _VertexNumber);
}

bool CreateMagicShaderObject(MGShader* _MGShader)
{
	*_MGShader = reinterpret_cast<MGShader>(new MagicShaderObject);
	if (!*_MGShader)
		return false;

	return true;
}

bool InitMagicShaderObject(const MG_VertexShader& _pMG_VertexShader, const MG_PixelShader& _pMG_PixelShader)
{
	return MagicGlobalContext::pMagicGlobalContext->GetNowMagicShaderObject()->Initialize(_pMG_VertexShader, _pMG_PixelShader);
}

void DeleteMagicShaderObject(MGShader* _MGShader)
{
	MagicGlobalContext::pMagicGlobalContext->DeleteObject(MG_Delete_Shader, *_MGShader);
	*_MGShader = 0;
}

void BindMagicShaderObject(const MGShader& _MGShader)
{
	MagicGlobalContext::pMagicGlobalContext->SetNowMagicShaderObject(reinterpret_cast<MagicShaderObject*>(_MGShader));
}

unsigned int MGAddShaderVariate(const unsigned int& _size)
{
	return MagicGlobalContext::pMagicGlobalContext->GetNowMagicShaderObject()->CreateVariate(_size);
}

void MG_SetShaderVariate(const unsigned int& _Pos, const void* _pValue)
{
	MagicGlobalContext::pMagicGlobalContext->GetNowMagicShaderObject()->MG_ShaderVariate(_Pos, _pValue);
}

void MG_Enable(const unsigned int& _Type)
{
	switch (_Type)
	{
	case MG_Alpha:
		MagicGlobalContext::pMagicGlobalContext->SetAlphaState(true);
		break;
	}
}

void MG_Disable(const unsigned int& _Type)
{
	switch (_Type)
	{
	case MG_Alpha:
		MagicGlobalContext::pMagicGlobalContext->SetAlphaState(false);
		break;
	}
}

bool MG_CreateBuffer(MGBuffer* _MGBuffer)
{
	*_MGBuffer = reinterpret_cast<MGBuffer>(new MagicVertexBufferObject);
	if (!*_MGBuffer)
		return false;

	return true;
}

void MG_DeleteBuffer(MGBuffer* _MGBuffer)
{
	MagicGlobalContext::pMagicGlobalContext->DeleteObject(MG_Delete_VertexBuffer, *_MGBuffer);
	*_MGBuffer = 0;
}

void MG_BindBuffer(const unsigned int& _mode, const MGBuffer& _MGBuffer)
{
	switch (_mode)
	{
	case MG_ARRAY_BUFFER:
		MagicGlobalContext::pMagicGlobalContext->SetNowMagicVertexBufferObject(reinterpret_cast<MagicVertexBufferObject*>(_MGBuffer));
		break;
	}
}

void MG_BufferData(const unsigned int& _mode, const unsigned int& _size, const void* _pdata)
{
	switch (_mode)
	{
	case MG_ARRAY_BUFFER:
		MagicGlobalContext::pMagicGlobalContext->GetNowMagicVertexBufferObject()->SetBufferData(_pdata, _size);
		break;
	}
}

void MG_VertexAttribPointer(unsigned char _pos, unsigned char _Size, const unsigned short& _type)
{
	MagicGlobalContext::pMagicGlobalContext->GetNowMagicVertexBuffer()->BindBuffer(_pos, MagicGlobalContext::pMagicGlobalContext->GetNowMagicVertexBufferObject());
	MagicGlobalContext::pMagicGlobalContext->GetNowMagicVertexBufferObject()->SetVertexAttribPointer(_Size, _type);
}

bool MG_CreateVertexArrays(MGVertexArrays* _MGVertexArrays)
{
	*_MGVertexArrays = reinterpret_cast<MGVertexArrays>(new MagicVertexBuffer);
	if (!*_MGVertexArrays)
		return false;

	return true;
}

void MG_DeleteVertexArrays(MGVertexArrays* _MGVertexArrays)
{
	MagicGlobalContext::pMagicGlobalContext->DeleteObject(MG_Delete_VertexArraysBuffer, *_MGVertexArrays);
	*_MGVertexArrays = 0;
}

void MG_BindVertexArrays(const MGVertexArrays& _MGVertexArrays)
{
	MagicGlobalContext::pMagicGlobalContext->SetNowMagicVertexBuffer(reinterpret_cast<MagicVertexBuffer*>(_MGVertexArrays));
}

void MG_EnableVertexAttribArray(unsigned char _number)
{
	MagicGlobalContext::pMagicGlobalContext->GetNowMagicVertexBuffer()->EnableVertexAttribArray(_number);
}

bool MG_CreateFrameBufferObject(MGFrameBuffer* _MGFrameBuffer)
{
	*_MGFrameBuffer = reinterpret_cast<MGFrameBuffer>(new MagicFrameBufferObject);
	if (!*_MGFrameBuffer)
		return false;

	return true;
}

void MG_DeleteFrameBufferObject(MGFrameBuffer* _MGFrameBuffer)
{
	MagicGlobalContext::pMagicGlobalContext->DeleteObject(MG_Delete_FrameBuffer, *_MGFrameBuffer);
	*_MGFrameBuffer = 0;
}

void MG_BindFrameBuffer(const MGFrameBuffer& _MGFrameBuffer)
{
	MagicGlobalContext::pMagicGlobalContext->SetNowMagicFrameBufferObject(reinterpret_cast<MagicFrameBufferObject*>(_MGFrameBuffer));
}

bool MG_InitFrameBuffer(const unsigned short& _width, const unsigned short& _height)
{
	if (!MagicGlobalContext::pMagicGlobalContext->GetNowMagicFrameBufferObject()->Initialize(_width, _height))
		return true;
	else
		return false;
}

void MG_ClearColor(const float& _r, const float& _g, const float& _b, const float& _a)
{
	MagicGlobalContext::pMagicGlobalContext->GetNowMagicFrameBufferObject()->SetClearColor(Magic::vector4(_r, _g, _b, _a));
}

void MG_Clear(const unsigned int& _mode)
{
	MagicGlobalContext::pMagicGlobalContext->ClearFrameBuffer(_mode);
}