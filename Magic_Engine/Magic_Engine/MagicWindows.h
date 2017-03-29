#pragma once

/*
类名:MagicWindows
描述:创建一个给定宽度和高度的Windows窗口
具体功能:
1.快速创建多功能的窗口
*/

//头文件声明:
#include "windows.h"
#include "MagicUIElemeent.h"
#include "MagicCircle.h"
#include "MagicAlphaWindow.h"
#include "SpecialEffectsPanel.h"

class MagicWindows
{
public:
	/*
	功能:构造函数
	参数:窗口名字,窗口宽度,窗口高度
	*/
	MagicWindows();
	~MagicWindows();

	bool Initialize();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);  //消息处理函数

	void Run();

private:
	bool CreateWindows(wchar_t* _Name, int _width, int _height);
	void Shutdown();

private:
	int m_ResultMessage;
	MagicEngineContext m_MagicEngineContext;
	MagicSceneCircle m_MagicSceneCircle;
	SpecialEffectsPanel m_SpecialEffectsPanel;

	SGDI m_SGDI;
	wchar_t m_applicationName[256];
	HINSTANCE m_hinstance;
	HWND m_hwnd;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//全局变量:
static MagicWindows* ApplicationHandle = 0;