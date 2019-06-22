#pragma once

/*
����:MagicWindows
����:����һ��������Ⱥ͸߶ȵ�Windows����
���幦��:
1.���ٴ����๦�ܵĴ���
*/

//ͷ�ļ�����:
#include "windows.h"
#include "MagicUIMacro.h"
#include "Magic_Thread.h"
#include "Define/Magic_Macro.h"


class MagicWindows
{
public:
	/*
	����:���캯��
	����:��������,���ڿ��,���ڸ߶�
	*/
	MagicWindows();
	~MagicWindows();

	static MagicWindows* Instance() { return pMagicWindows; }

	bool Initialize(const wchar_t* _name, int _x, int _y, int _w, int _h);

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);  //��Ϣ������

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