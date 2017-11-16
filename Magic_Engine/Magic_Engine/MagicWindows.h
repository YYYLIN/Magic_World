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
#include "MagicGratingAPI.h"
#include "MagicWorld.h"
#include "MainScene.h"

#pragma comment(lib,"MagicGrating.lib")

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
	//	MagicSceneCircle m_MagicSceneCircle;
	MagicWorld m_MagicWorld;
	MainScene m_MagicScene;
	SpecialEffectsPanel m_SpecialEffectsPanel;

	/*
		MGContext m_MGContext;
		MGShader m_Shader;
		MGVertexArrays m_VertexArrays;
		MGBuffer m_VertexBuffer;
		MGFrameBuffer m_MGFrameBuffer;
		unsigned int m_ShaderCamera;
		unsigned int m_Shaderprojection;
		unsigned int m_Shaderworld;*/

	SGDI m_SGDI;
	wchar_t m_applicationName[256];
	HINSTANCE m_hinstance;
	HWND m_hwnd;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//ȫ�ֱ���:
static MagicWindows* ApplicationHandle = 0;