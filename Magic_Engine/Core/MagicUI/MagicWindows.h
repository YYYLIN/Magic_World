#pragma once

/*
类名:MagicWindows
描述:创建一个给定宽度和高度的Windows窗口
具体功能:
1.快速创建多功能的窗口
*/

//头文件声明:
#include "windows.h"
#include "MagicUIMacro.h"
#include "Magic_Thread.h"
#include "Define/Magic_Macro.h"


class MagicWindows
{
public:
	/*
	功能:构造函数
	参数:窗口名字,窗口宽度,窗口高度
	*/
	MagicWindows();
	~MagicWindows();

	static MagicWindows* Instance() { return pMagicWindows; }

	bool Initialize(const wchar_t* _name, int _x, int _y, int _w, int _h);

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);  //消息处理函数

	void Updata();

	inline HWND GetHWND() { return m_hwnd; }
	inline HINSTANCE GetHINSTANCE() { return m_hinstance; }

	void SetCallbackMessage_WIN32(Magic::CallbackMessage_WIN32 _CallbackMessage_WIN32);
	void RequestOuitSystem();
private:
	bool CreateWindows(const wchar_t* _Name, int _x, int _y, int _width, int _height);
	void Shutdown(Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message);

private:
	Magic::CallbackMessage_WIN32 m_CallbackMessage_WIN32;

	wchar_t m_applicationName[256];
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	RenderContext m_RenderContext;

	static MagicWindows* pMagicWindows;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);