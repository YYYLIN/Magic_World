
/*
Source File:MagicGlobalVariable.h
IOFO:Rendering interface
author:YYYLIN
Create time:2017 04 16
*/

#ifndef _MAGICGLOBALVARIABLE_H_
#define _MAGICGLOBALVARIABLE_H_

#include <process.h>
#include <windows.h>
#include "MagicVertexBufferObject.h"
#include "MagicShaderObject.h"
#include "MagicFrameBufferObject.h"
#include "DynamicQueue.hpp"

struct DrawObject
{
	unsigned int MessageMode;
	union
	{
		struct
		{
			unsigned int DrawMode;
			unsigned int VertexStart;
			unsigned int VertexNumber;
			void* pVertexVariate;
		};

		struct
		{
			unsigned int ClearMode;
			unsigned int Placeholder1;
			unsigned int Placeholder2;
			unsigned int Placeholder3;
		};

		struct
		{
			unsigned int DeleteObjectType;
			MGObjectType DeleteObjectData;
			unsigned int Placeholder2;
			unsigned int Placeholder3;
		};
	};

	MagicShaderObject* pMagicShaderObject;
	MagicVertexBuffer* pMagicVertexBuffer;
	MagicFrameBufferObject* pMagicFrameBufferObject;
	bool AlphaState;
};

struct DisplayerData
{
	void* pBuffer;
	unsigned short m_Width, m_Height;
	unsigned short m_Mode, m_Type;
};

/*
功能:光棚化渲染器
*/
class MagicGlobalContext
{
public:
	MagicGlobalContext();
	~MagicGlobalContext();

	bool Initialize();
	void Shutdown();

	inline MagicShaderObject* GetNowMagicShaderObject() { return m_NowDrawObject.pMagicShaderObject; }
	inline MagicVertexBufferObject* GetNowMagicVertexBufferObject() { return pNowMagicVertexBufferObject; }
	inline MagicVertexBuffer* GetNowMagicVertexBuffer() { return m_NowDrawObject.pMagicVertexBuffer; }
	inline MagicFrameBufferObject* GetNowMagicFrameBufferObject() { return m_NowDrawObject.pMagicFrameBufferObject; }
	void SetNowMagicShaderObject(MagicShaderObject* _pMagicShaderObject);
	void SetNowMagicVertexBufferObject(MagicVertexBufferObject* _pMagicVertexBufferObject);
	void SetNowMagicVertexBuffer(MagicVertexBuffer* _pMagicVertexBuffer);
	void SetNowMagicFrameBufferObject(MagicFrameBufferObject* _pMagicFrameBufferObject);
	void SetAlphaState(bool _state);
	void SetDisplayerData(const DisplayerData& _DisplayerData);

	void AddDrawObject(const unsigned int& _DrawMode, const unsigned int& _VertexStart, const unsigned int&  _VertexNumber);
	void ClearFrameBuffer(const unsigned int& _mode);
	void DeleteObject(const unsigned int& _deleteType, const MGObjectType& _deleteData);

	void ShutdownGlobalContext();

	void WaitSwapFrame();

private:
	void DisplayerCopy();

	static DWORD WINAPI GlobalThread(LPVOID lpParameter);

	void RenderHandle(const DrawObject& _DrawObject);
	void DeleteHandle(const DrawObject& _DrawObject);
	void ClearHandle(const DrawObject& _DrawObject);

	void ShutdownHandle();
private:
	HANDLE m_ContextThread;
	HANDLE m_RequertsSemaphore;
	HANDLE m_RequertsFinish;
	CRITICAL_SECTION m_QueueLock;

	DrawObject m_NowDrawObject;

	MagicVertexBufferObject* pNowMagicVertexBufferObject;

	DynamicQueue<DrawObject> Queue_DrawObject;

	DisplayerData m_DisplayerData;

	bool m_ShutdownState;

public:
	static MagicGlobalContext* pMagicGlobalContext;
};

#endif