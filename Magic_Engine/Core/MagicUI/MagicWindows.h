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
#include "MagicEngineContext.h"
#include "MagicUIMacro.h"

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

	bool Initialize(const char* _name, int _x, int _y, int _w, int _h);

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);  //��Ϣ������

	void Run();

private:
	bool CreateWindows(const char* _Name, int _x, int _y, int _width, int _height);
	void Shutdown();

private:
	int m_ResultMessage;
	MagicEngineContext m_MagicEngineContext;
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
	char m_applicationName[256];
	HINSTANCE m_hinstance;
	HWND m_hwnd;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//ȫ�ֱ���:
static MagicWindows* ApplicationHandle = 0;