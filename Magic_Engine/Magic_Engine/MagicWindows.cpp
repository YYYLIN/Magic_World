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
	// �õ�һ���ⲿָ��ָ���������
	ApplicationHandle = this;

	// �õ����Ӧ�ó����ʵ����
	m_hinstance = GetModuleHandle(NULL);

	// ��װWindows���Ĭ�����á�
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = (HICON)::LoadImage(NULL, L"ħ������.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// ע�ᴰ���ࡣ
	RegisterClassEx(&wc);

	posX = (GetSystemMetrics(SM_CXSCREEN) - _width) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - _height) / 2;

	// ����Ļ�����ô������ڲ���������
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_POPUP | WS_MINIMIZEBOX | WS_SYSMENU,
		posX, posY, _width, _height, NULL, NULL, m_hinstance, NULL);

	// �Ѵ�������Ļ�ϲ���������Ϊ���㡣
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	return true;
}

LRESULT CALLBACK MagicWindows::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)    //��Ϣ������
{
	switch (umsg)
	{
		// ����Ƿ��м������¼����ϵġ�
	case WM_KEYDOWN:
	{
		if (wparam == VK_ESCAPE)    // ��������µļ���ESC
			DestroyWindow(hwnd);		// ���ٴ���, ������һ��WM_DESTROY��Ϣ
		m_MagicWorld.MessageHandle(wparam, lparam);
		return 0;
	}

	// ����Ƿ����ڼ������ͷŵĹؼ���
	case WM_KEYUP:
	{
		return 0;
	}

	// �κ���������Ϣ���͵�Ĭ�ϵ���Ϣ�������Ϊ���ǵ�Ӧ�ó���ʹ�����ǡ�
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

void MagicWindows::Run()     //���̴�����
{
	MSG msg;
	bool done;


	// ��ʼ����Ϣ�ṹ��
	ZeroMemory(&msg, sizeof(MSG));

	// ѭ��ֱ���дӴ��ڻ��û��˳���Ϣ��
	done = false;
	while (!done)
	{
		// ����Windows��Ϣ��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// ���Windows�źŶ˵�Ӧ��Ȼ���˳���
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
	// ɾ�����ڡ�
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// ɾ��Ӧ��ʵ����
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// �ͷ�ָ��ָ������ࡣ
	ApplicationHandle = NULL;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)    //��Ϣ������
{
	switch (umessage)
	{
		// ������ڱ����١�
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// ������ڱ��رա�
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

		// ������������Ϣ����ϵͳ�е������Ϣ�������
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}