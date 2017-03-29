#pragma once

/*
����:MagicWindows
����:����һ��������Ⱥ͸߶ȵ�Windows����
���幦��:
1.���ٴ����๦�ܵĴ���
*/

//ͷ�ļ�����:
#include "windows.h"
#include "MagicUIElemeent.h"
#include "MagicCircle.h"
#include "MagicAlphaWindow.h"
#include "SpecialEffectsPanel.h"

class MagicWindows
{
public:
	/*
	����:���캯��
	����:��������,���ڿ��,���ڸ߶�
	*/
	MagicWindows();
	~MagicWindows();

	bool Initialize();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);  //��Ϣ������

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

//ȫ�ֱ���:
static MagicWindows* ApplicationHandle = 0;