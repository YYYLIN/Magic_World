#pragma once

/*
����:MagicWindows
����:����һ��������Ⱥ͸߶ȵ�Windows����
���幦��:
1.���ٴ����๦�ܵĴ���
*/

//ͷ�ļ�����:
#include "windows.h"
/*#include "MagicAlphaWindow.h"*/
/*#include "MagicGratingAPI.h"*/
#include "MagicUIMacro.h"
#include "UserScene.h"
#include "MenuScene.h"
#include "DebugScene.h"
#include "SystemScene.h"

/*#pragma comment(lib,"MagicGrating.lib")*/

class MagicWindows
{
public:
	/*
	����:���캯��
	����:��������,���ڿ��,���ڸ߶�
	*/
	MagicWindows();
	~MagicWindows();

	bool Initialize(const wchar_t* _name, int _x, int _y, int _w, int _h);

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);  //��Ϣ������

	void Run();

	MagicScenes* GetSystemScenes(Magic::SYSTEMSCENES _SYSTEMSCENES);

	inline HWND GetHWND(){ return m_hwnd; }
	inline HINSTANCE GetHINSTANCE(){ return m_hinstance; }

	void SetCallbackMessage_WIN32(Magic::CallbackMessage_WIN32 _CallbackMessage_WIN32);
	void RequestOuitSystem();
private:
	bool CreateWindows(const wchar_t* _Name, int _x, int _y, int _width, int _height);
	void Shutdown();

private:
	MagicEngineContext m_MagicEngineContext;
	Magic::UI::UserScene m_UserScene;
	Magic::UI::MenuScene m_MenuScene;
	Magic::UI::DebugScene m_DebugScene;
	Magic::UI::SystemScene m_SystemScene;
	Magic::CallbackMessage_WIN32 m_CallbackMessage_WIN32;
	//	MagicSceneCircle m_MagicSceneCircle;

	/*
		MGContext m_MGContext;
		MGShader m_Shader;
		MGVertexArrays m_VertexArrays;
		MGBuffer m_VertexBuffer;
		MGFrameBuffer m_MGFrameBuffer;
		unsigned int m_ShaderCamera;
		unsigned int m_Shaderprojection;
		unsigned int m_Shaderworld;*/

	/*	SGDI m_SGDI;*/
	wchar_t m_applicationName[256];
	HINSTANCE m_hinstance;
	HWND m_hwnd;
public:
	static MagicWindows* pMagicWindows;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);