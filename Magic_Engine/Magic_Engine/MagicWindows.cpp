#include "MagicWindows.h"

struct VertexShaderType
{
	Magic::vector3 m_pos;
};

Magic::vector4 Shader_VertexShader(void* const pVertex, void* const pVariate)
{
	VertexShaderType* pVertexShaderType = (VertexShaderType*)pVertex;
	return Magic::vector4(pVertexShaderType->m_pos.x, pVertexShaderType->m_pos.y, pVertexShaderType->m_pos.z, 1.0f);
}

Magic::vector4 Shader_PixelShader(void* const pIn, void* const pVariate)
{
	return Magic::vector4(1.0f, 0, 0, 1);
}

MagicWindows::MagicWindows()
{

}


MagicWindows::~MagicWindows()
{
	Shutdown();
}

bool MagicWindows::Initialize()
{
	bool result;
	result = CreateWindows(L"MagicWindow", 1024, 768);
	if (!result)
		return false;

	result = m_MagicEngineContext.Initialize(m_hwnd, 0, 0, 1024, 768);
	if (!result)
		return false;

	/*
		result = EnableAlphaWindow(m_hwnd, 1024, 768, &m_SGDI);
		if (!result)
			return false;*/


	m_MagicEngineContext.SetBackColor(1.0f, 1.0f, 1.0f, 1.0f);

	result = m_MagicWorld.Initialize();
	if (!result)
		return false;
	m_MagicEngineContext.AddCommon(&m_MagicWorld);

	/*
		result = m_MagicSceneCircle.MagicScene::Initialize(glm::vec4(0, 0, 1024, 768), &m_MagicEngineContext);
		if (!result)
			return false;*/



			/*
				result = m_SpecialEffectsPanel.Initialize(1024, 768);
				if (!result)
					return false;*/
					//--------------------------------------------
				/*
					result = CreateMagicGratingContext(&m_MGContext);
					if (!result)
						return false;
					result = InitMagicGratingContext(m_MGContext);
					if (!result)
						return false;
					BindMagicGratingContext(m_MGContext);
					BindMGDisplayContext(m_MGContext, m_SGDI.m_pBackBuffer, 1024, 768, MG_BGRA, MG_UCHAR);
					//--------------------------------------------
					MG_CreateFrameBufferObject(&m_MGFrameBuffer);
					MG_BindFrameBuffer(m_MGFrameBuffer);
					result = MG_InitFrameBuffer(1024, 768);
					if (!result)
						return false;
					MG_ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
					//-------------Shader-------------------------
					CreateMagicShaderObject(&m_Shader);
					BindMagicShaderObject(m_Shader);
					m_ShaderCamera = MGAddShaderVariate(sizeof(Magic::Matrix4));
					m_Shaderprojection = MGAddShaderVariate(sizeof(Magic::Matrix4));
					m_Shaderworld = MGAddShaderVariate(sizeof(Magic::Matrix4));
					InitMagicShaderObject(Shader_VertexShader, Shader_PixelShader);

					MG_SetShaderVariate(m_ShaderCamera, &Magic::Matrix4());
					MG_SetShaderVariate(m_Shaderprojection, &Magic::Matrix4());
					MG_SetShaderVariate(m_Shaderworld, &Magic::Matrix4());
					//--------------------------------------------
					MG_CreateVertexArrays(&m_VertexArrays);
					MG_BindVertexArrays(m_VertexArrays);
					MG_EnableVertexAttribArray(1);

					float pVertex[] =
					{
						0.5f,0.0f,0.0f,
						0.0f,0.5f,0.0f,
						-0.5f,0.0f,0.0f
					};

					MG_CreateBuffer(&m_VertexBuffer);
					MG_BindBuffer(MG_ARRAY_BUFFER, m_VertexBuffer);
					MG_BufferData(MG_ARRAY_BUFFER, sizeof(float) * 3 * 3, pVertex);
					MG_VertexAttribPointer(0, 3, MG_FLOAT);*/

	return true;
}

bool MagicWindows::CreateWindows(wchar_t* _Name, int _width, int _height)
{
	WNDCLASSEX wc;
	int posX, posY;
	wcscpy(m_applicationName, _Name);
	// 得到一个外部指针指向这个对象。
	ApplicationHandle = this;

	// 得到这个应用程序的实例。
	m_hinstance = GetModuleHandle(NULL);

	// 安装Windows类的默认设置。
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = (HICON)::LoadImage(NULL, L"魔法阵：鑫.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //本地加载自定义ico图标
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// 注册窗口类。
	RegisterClassEx(&wc);

	posX = (GetSystemMetrics(SM_CXSCREEN) - _width) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - _height) / 2;

	// 在屏幕上设置创建窗口并掌握它。
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_POPUP | WS_MINIMIZEBOX | WS_SYSMENU,
		posX, posY, _width, _height, NULL, NULL, m_hinstance, NULL);

	// 把窗口在屏幕上并将它设置为焦点。
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	return true;
}

LRESULT CALLBACK MagicWindows::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)    //消息处理函数
{
	switch (umsg)
	{
		// 检查是否有键被按下键盘上的。
	case WM_KEYDOWN:
	{
		if (wparam == VK_ESCAPE)    // 如果被按下的键是ESC
			DestroyWindow(hwnd);		// 销毁窗口, 并发送一条WM_DESTROY消息
		m_MagicWorld.MessageHandle(wparam, lparam);
		return 0;
	}

	// 检查是否已在键盘上释放的关键。
	case WM_KEYUP:
	{
		return 0;
	}

	// 任何其他的消息发送到默认的消息处理程序为我们的应用程序不使用它们。
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

void MagicWindows::Run()     //过程处理函数
{
	MSG msg;
	bool done;


	// 初始化消息结构。
	ZeroMemory(&msg, sizeof(MSG));

	// 循环直到有从窗口或用户退出消息。
	done = false;
	while (!done)
	{
		// 处理Windows消息。
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 如果Windows信号端的应用然后退出。
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{

			m_MagicEngineContext.Render();

			/*
						glReadPixels(0, 0, m_SGDI.m_Width, m_SGDI.m_Height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, m_SGDI.m_pBackBuffer);
						glBindTexture(GL_TEXTURE_2D, m_MagicSceneCircle.GetTextrue());
						glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, m_SGDI.m_pBackBuffer);*/
						/*
									MG_Clear(MG_COLOR_BUFFER);

									MGDrawArrays(MG_DrawPoints, 0, 3);

									MG_WaitSwapFrame();

									UpdataAlphaWindow(&m_SGDI);*/
		}
	}

	return;

}

void MagicWindows::Shutdown()
{
	/*
		MG_DeleteBuffer(&m_VertexBuffer);
		MG_DeleteVertexArrays(&m_VertexArrays);
		DeleteMagicShaderObject(&m_Shader);
		MG_DeleteFrameBufferObject(&m_MGFrameBuffer);
		DeleteMagicGratingContext(&m_MGContext);*/

	DisableAlphaWindow(&m_SGDI);
	// 删除窗口。
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// 删除应用实例。
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// 释放指针指向这个类。
	ApplicationHandle = NULL;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)    //消息处理函数
{
	switch (umessage)
	{
		// 如果窗口被销毁。
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// 如果窗口被关闭。
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

		// 所有其它的消息传递系统中的类的消息处理程序。
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}