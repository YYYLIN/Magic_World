
/*
Source File:MagicGlobalVariable.cpp
IOFO:Rendering interface
author:YYYLIN
Create time:2017 04 16
*/

#include "MagicGlobalVariable.h"
#include "MagicMacro.h"
#include "MagicInsideMacro.h"

MagicGlobalContext* MagicGlobalContext::pMagicGlobalContext = 0;

MagicGlobalContext::MagicGlobalContext()
{
}

MagicGlobalContext::~MagicGlobalContext()
{
	Shutdown();
}

bool MagicGlobalContext::Initialize()
{
	m_ShutdownState = false;
	InitializeCriticalSection(&m_QueueLock);

	m_RequertsSemaphore = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
	m_RequertsFinish = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
	m_ContextThread = (HANDLE)CreateThread(NULL, 0, GlobalThread, (LPVOID)this, 0, NULL);

	return true;
}

void MagicGlobalContext::Shutdown()
{
	WaitForSingleObject(m_ContextThread, INFINITE);

	CloseHandle(m_ContextThread);
	CloseHandle(m_RequertsFinish);
	CloseHandle(m_RequertsSemaphore);
	DeleteCriticalSection(&m_QueueLock);
}

void MagicGlobalContext::SetNowMagicShaderObject(MagicShaderObject* _pMagicShaderObject)
{
	m_NowDrawObject.pMagicShaderObject = _pMagicShaderObject;
}

void MagicGlobalContext::SetNowMagicVertexBufferObject(MagicVertexBufferObject* _pMagicVertexBufferObject)
{
	pNowMagicVertexBufferObject = _pMagicVertexBufferObject;
}

void MagicGlobalContext::SetNowMagicVertexBuffer(MagicVertexBuffer* _pMagicVertexBuffer)
{
	m_NowDrawObject.pMagicVertexBuffer = _pMagicVertexBuffer;
}

void MagicGlobalContext::SetNowMagicFrameBufferObject(MagicFrameBufferObject* _pMagicFrameBufferObject)
{
	m_NowDrawObject.pMagicFrameBufferObject = _pMagicFrameBufferObject;
}

void MagicGlobalContext::SetAlphaState(bool _state)
{
	m_NowDrawObject.AlphaState = _state;
}

void MagicGlobalContext::SetDisplayerData(const DisplayerData& _DisplayerData)
{
	m_DisplayerData = _DisplayerData;
}

void MagicGlobalContext::AddDrawObject(const unsigned int& _DrawMode, const unsigned int& _VertexStart, const unsigned int&  _VertexNumber)
{
	m_NowDrawObject.MessageMode = MG_Message_Draw;
	m_NowDrawObject.DrawMode = _DrawMode;
	m_NowDrawObject.VertexStart = _VertexStart;
	m_NowDrawObject.VertexNumber = _VertexNumber;
	m_NowDrawObject.pVertexVariate = m_NowDrawObject.pMagicShaderObject->GetNEW_NowVariate();
	EnterCriticalSection(&m_QueueLock);
	Queue_DrawObject << m_NowDrawObject;
	LeaveCriticalSection(&m_QueueLock);

	ReleaseSemaphore(m_RequertsSemaphore, 1, NULL);
}

void MagicGlobalContext::ClearFrameBuffer(const unsigned int& _mode)
{
	m_NowDrawObject.MessageMode = MG_Message_Clear;
	m_NowDrawObject.ClearMode = _mode;
	EnterCriticalSection(&m_QueueLock);
	Queue_DrawObject << m_NowDrawObject;
	LeaveCriticalSection(&m_QueueLock);

	ReleaseSemaphore(m_RequertsSemaphore, 1, NULL);
}

void MagicGlobalContext::DeleteObject(const unsigned int& _deleteType, const MGObjectType& _deleteData)
{
	m_NowDrawObject.MessageMode = MG_Message_Delete;
	m_NowDrawObject.DeleteObjectType = _deleteType;
	m_NowDrawObject.DeleteObjectData = _deleteData;

	EnterCriticalSection(&m_QueueLock);
	Queue_DrawObject << m_NowDrawObject;
	LeaveCriticalSection(&m_QueueLock);

	ReleaseSemaphore(m_RequertsSemaphore, 1, NULL);
}

void MagicGlobalContext::ShutdownGlobalContext()
{
	m_NowDrawObject.MessageMode = MG_Message_Shutdown;

	EnterCriticalSection(&m_QueueLock);
	Queue_DrawObject << m_NowDrawObject;
	m_ShutdownState = true;
	LeaveCriticalSection(&m_QueueLock);

	ReleaseSemaphore(m_RequertsSemaphore, 1, NULL);

	WaitForSingleObject(m_ContextThread, INFINITE);
}

void MagicGlobalContext::WaitSwapFrame()
{
	m_NowDrawObject.MessageMode = MG_Message_DisplayCopy;

	EnterCriticalSection(&m_QueueLock);
	Queue_DrawObject << m_NowDrawObject;
	LeaveCriticalSection(&m_QueueLock);

	ReleaseSemaphore(m_RequertsSemaphore, 1, NULL);
	WaitForSingleObject(m_RequertsFinish, INFINITE);
}

void MagicGlobalContext::DisplayerCopy()
{
	m_NowDrawObject.pMagicFrameBufferObject->Copy(m_DisplayerData.pBuffer, m_DisplayerData.m_Width, m_DisplayerData.m_Height, m_DisplayerData.m_Mode, m_DisplayerData.m_Type);
}

DWORD WINAPI MagicGlobalContext::GlobalThread(LPVOID lpParameter)
{
	MagicGlobalContext* _pMagicGlobalContext = (MagicGlobalContext*)lpParameter;
	bool _ShutdownState = false;
	while (!_ShutdownState)
	{
		DrawObject _DrawObject;
		WaitForSingleObject(_pMagicGlobalContext->m_RequertsSemaphore, INFINITE);

		EnterCriticalSection(&_pMagicGlobalContext->m_QueueLock);
		_pMagicGlobalContext->Queue_DrawObject >> &_DrawObject;
		_ShutdownState = _pMagicGlobalContext->m_ShutdownState;
		LeaveCriticalSection(&_pMagicGlobalContext->m_QueueLock);


		switch (_DrawObject.MessageMode)
		{
		case MG_Message_Draw:
			_pMagicGlobalContext->RenderHandle(_DrawObject);
			break;
		case MG_Message_Delete:
			_pMagicGlobalContext->DeleteHandle(_DrawObject);
			break;
		case MG_Message_Clear:
			_pMagicGlobalContext->ClearHandle(_DrawObject);
			break;
		case MG_Message_DisplayCopy:
			_pMagicGlobalContext->DisplayerCopy();
			ReleaseSemaphore(_pMagicGlobalContext->m_RequertsFinish, 1, NULL);
			break;
		}
	}

	_pMagicGlobalContext->ShutdownHandle();

	return 0;
}

void MagicGlobalContext::RenderHandle(const DrawObject& _DrawObject)
{
	//创建结果缓存顶点
	Magic::vector4* _pResultVertex = new Magic::vector4[_DrawObject.VertexNumber];

	//顶点处理
	MG_VertexShader _MG_VertexShader = _DrawObject.pMagicShaderObject->GetMG_VertexShader();
	MagicVertexBuffer* _pMagicVertexBuffer = _DrawObject.pMagicVertexBuffer;

	unsigned int _VertexSize = 0;
	for (int a = 0;a < _pMagicVertexBuffer->GetNumber();a++)
		_VertexSize += (*_pMagicVertexBuffer)[a]->GetTypeSize();

	unsigned char* _pVertexData = (unsigned char*)malloc(_VertexSize);
	for (int v = _DrawObject.VertexStart, _endVertex = _DrawObject.VertexStart + _DrawObject.VertexNumber
		;v < _endVertex;v++)
	{
		unsigned int _NowVertexDataIndex = 0;
		for (int a = 0;a < _pMagicVertexBuffer->GetNumber();a++)
		{
			(*_pMagicVertexBuffer)[a]->GetVertex(v, &_pVertexData[_NowVertexDataIndex]);
			_NowVertexDataIndex += (*_pMagicVertexBuffer)[a]->GetTypeSize();
		}

		_pResultVertex[v - _DrawObject.VertexStart] = _MG_VertexShader(_pVertexData, _DrawObject.pVertexVariate);
	}
	free(_pVertexData);

	//光棚化处理

	//像素处理
	MagicFrameBufferObject* _pMagicFrameBufferObject = _DrawObject.pMagicFrameBufferObject;
	MG_PixelShader _MG_PixelShader = _DrawObject.pMagicShaderObject->GetMG_PixelShader();
	switch (_DrawObject.DrawMode)
	{
	case MG_DrawPoints:
		for (unsigned int v = 0;v < _DrawObject.VertexNumber;v++)
		{
			_pMagicFrameBufferObject->SetPosColor(
				_pResultVertex[v].x, _pResultVertex[v].y, _MG_PixelShader(0, _DrawObject.pVertexVariate));
		}
		break;
	}

	delete[]_pResultVertex;
	free(_DrawObject.pVertexVariate);
}

void MagicGlobalContext::DeleteHandle(const DrawObject& _DrawObject)
{
	switch (_DrawObject.DeleteObjectType)
	{
	case MG_Delete_FrameBuffer:
		delete reinterpret_cast<MagicFrameBufferObject*>(_DrawObject.DeleteObjectData);
		break;
	case MG_Delete_VertexArraysBuffer:
		delete reinterpret_cast<MagicVertexBuffer*>(_DrawObject.DeleteObjectData);
		break;
	case MG_Delete_VertexBuffer:
		delete reinterpret_cast<MagicVertexBufferObject*>(_DrawObject.DeleteObjectData);
		break;
	case MG_Delete_Shader:
		delete reinterpret_cast<MagicShaderObject*>(_DrawObject.DeleteObjectData);
		break;
	}
}

void MagicGlobalContext::ClearHandle(const DrawObject& _DrawObject)
{
	_DrawObject.pMagicFrameBufferObject->Clear(_DrawObject.ClearMode);
}

void MagicGlobalContext::ShutdownHandle()
{
	while (Queue_DrawObject.GetSize())
	{
		DrawObject _DrawObject;
		Queue_DrawObject >> &_DrawObject;
		if (_DrawObject.MessageMode == MG_Message_Delete)
			DeleteHandle(_DrawObject);
	}
}